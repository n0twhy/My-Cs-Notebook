// =============================================================================
// TinyRPC 调度器骨架（只有签名 + TODO，自己填实现）
//
// 对应关系（本文件用的名字 ←→ TinyRPC 源码里的名字）：
//   MyReactor        ↔  tinyrpc::Reactor
//   GlobalCoroQueue  ↔  tinyrpc::CoroutineTaskQueue
//   FdEvent          ↔  tinyrpc::FdEvent（直接沿用）
//   Coroutine        ↔  tinyrpc::Coroutine（直接沿用）
//
// 术语约束（填实现时请保持以下语义，否则会和 tinyrpc 原意偏差）：
//   · 每个 IO 线程一个 MyReactor，MyReactor::loop() 只允许它自己的线程调。
//   · epoll_event.data.ptr 永远存 FdEvent*，不存 Coroutine*。
//   · "第一个就绪协程留本地 Resume，其余全部扔 GlobalCoroQueue" —— 无条件策略。
//   · push/pop GlobalCoroQueue 必须加锁；本地 m_pending_tasks 进出也要锁。
//   · loop 一圈顺序固定：A(上轮预约) → B(抢公债) → C(本地零活) → D(epoll_wait) → E(分发)
// =============================================================================

#ifndef MY_REACTOR_SKELETON_H
#define MY_REACTOR_SKELETON_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <mutex>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include <map>
#include <atomic>
#include <functional>
#include <memory>
#include "tinyrpc/coroutine/coroutine.h"
#include "tinyrpc/net/fd_event.h"

// 假装已经有这几个类型（沿用 tinyrpc 的）
namespace tinyrpc {
  class Coroutine;
  class FdEvent;
  class Timer;
  class Mutex;
}

namespace myrpc {
// -----------------------------------------------------------------------------
// 全局协程任务队列（跨线程窃取用）
// -----------------------------------------------------------------------------
//
// 问自己：
//   Q1. 为什么用 std::queue<FdEvent*>，不是 std::queue<Coroutine*>？
//       （提示：回想"所有权迁移"和 epoll_event.data.ptr 只有一个指针位）
//   Q2. push 需要加锁，pop 也需要加锁。能不能做成无锁？要付出什么代价？
//   Q3. pop() 空时返回 nullptr 而不是阻塞——为什么？阻塞会死锁在哪里？
// -----------------------------------------------------------------------------

class MyReactor;
extern thread_local MyReactor *t_reactor;

class GlobalCoroQueue {
  struct Node {
    tinyrpc::FdEvent *data;
    std::atomic<Node *> next {nullptr};
  };
 public:
  // 单例：整个进程一份
  static GlobalCoroQueue* GetInstance();

  // 把一个就绪 FdEvent 塞进全局队列
  // 调用方要求：已经调过 fd_event->setReactor(nullptr) 解绑原 Reactor
  void push(tinyrpc::FdEvent* fd_event);

  // 弹出队头。空则返回 nullptr（非阻塞）
  tinyrpc::FdEvent* pop();

 private:
  GlobalCoroQueue() = default;

  // TODO: 用什么容器？
  //   - std::queue<FdEvent*>? 还是 std::deque? 差别是什么？
  //   - 要不要考虑 moodycamel::ConcurrentQueue 这种无锁队列？
  std::atomic<Node *> m_head;
  std::atomic<Node *> m_tail;

  // TODO: Mutex 保护上面这个队列
  // tinyrpc::Mutex m_mutex;
};


// -----------------------------------------------------------------------------
// 每线程一个的 Reactor
// -----------------------------------------------------------------------------
//
// 线程模型：
//   · 构造时记录 pthread tid；之后 loop() 只能被这个 tid 调用，
//     assert(isLoopThread()) 在 loop() 开头守门。
//   · addEvent/addTask/addCoroutine 可以被"别的线程"调（典型：主Reactor accept
//     完新连接后，把 client fd 交给某个 SubReactor）—— 所以它们内部要
//     入 m_pending_add_fds / m_pending_tasks 延迟到 loop 线程处理，然后 wakeup。
//
// 问自己：
//   Q4. m_pending_tasks 和 GlobalCoroQueue 都是"待跑的东西"，差别是什么？
//       （提示：前者装 std::function，后者装 FdEvent*；前者是"本线程专属"，
//        后者是"任何 SubReactor 都能抢"）
//   Q5. m_fds 这个 vector 有什么用？和 EPOLL 内核注册表是同一个东西吗？
// -----------------------------------------------------------------------------

enum MyReactorType {
  kMainReactor = 1,   // 只负责 accept
  kSubReactor  = 2,   // IO 线程，每个 IO 线程一个
};

class MyReactor {
 public:
  using ptr = std::shared_ptr<MyReactor>;

  explicit MyReactor() {
    m_tid = getTid();
    m_epfd = epoll_create1(EPOLL_CLOEXEC);
    if (m_epfd == -1) {
      close(m_epfd);
      throw std::runtime_error("failed to create epoll");
    }

    m_stop_flag = false;
    m_is_looping = false;
    
    m_wake_fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    addWakeupFd();

    myrpc::t_reactor = this;
  };
  ~MyReactor() {
    close(m_epfd);
    close(m_wake_fd);
    close(m_timer_fd);

    myrpc::t_reactor = nullptr;
  }

  // -------- 外部 API（可能被其他线程调）--------
  // 把 fd 注册到 epoll 监听 event。is_wakeup=true 表示跨线程时写一字节
  // 到 m_wake_fd 把 loop 线程从 epoll_wait 中唤醒。
  void addEvent(int fd, epoll_event event, bool is_wakeup = true) {
    if (isLoopThread()) {
      addEventInLoopThread(fd, event);
    } else {
      std::lock_guard<std::mutex> lock(m_mtx);
      m_pending_add_fds[fd] = event;
      if (is_wakeup) {
        wakeup();
      }
    }
  }

  void delEvent(int fd, bool is_wakeup = true);

  // 丢一个回调到本 Reactor 的本地任务队列
  void addTask(std::function<void()> task, bool is_wakeup = true) {
    m_pending_tasks.emplace_back(task);
  }

  // 丢一个协程，本质是把"Resume(cor)"包成 lambda 丢进 m_pending_tasks
  void addCoroutine(std::shared_ptr<tinyrpc::Coroutine> cor,
                    bool is_wakeup = true);

  // 往 m_wake_fd 写一字节，强行让 epoll_wait 返回
  void wakeup() {
    if (!m_is_looping) {
      return;
    }
    
    char tmp = 1;
    write(m_wake_fd, &tmp, 1);
    return;
  }

  // -------- 主事件循环（只能被 loop 线程自己调）--------
  void loop() {
    tinyrpc::Coroutine::ptr::first_cortoutine = nullptr;
    m_stop_flag = false;
    
    while (!m_stop_flag) {
      if(first_cortoutine) {
        tinyrpc::Coroutine::Resume(first_cortoutine.get());
      }

    while (tinyrpc::CoroutineTaskQueue::getInstance)
      
    }
  }
  void stop() {
    m_stop_flag = true;
    return;
  }

  // -------- 工具 --------
  tinyrpc::Timer* getTimer();
  pid_t getTid() const;
  void setReactorType(MyReactorType type);

  // 每个线程返回它自己的 Reactor 指针（thread_local 实现）
  static MyReactor* GetCurrentReactor();

 private:
  // -------- 仅 loop 线程内部调用 --------
  bool isLoopThread() const {
    return m_tid == getTid();
  }
  void addWakeupFd() {
    epoll_event ep_event;
    ep_event.events = EPOLLIN;
    ep_event.data.fd = m_wake_fd;
    int op = EPOLL_CTL_ADD;
    epoll_ctl(m_epfd, op, ep_event.data.fd, &ep_event);
    m_fds.emplace_back(m_wake_fd);
  }
  void addEventInLoopThread(int fd, epoll_event event) {
    std::lock_guard<std::mutex> lock(m_mtx);
      auto it = find(m_fds.begin(), m_fds.end(), fd);
      if (it != m_fds.end()) {
        int op = EPOLL_CTL_MOD;
        int rt = epoll_ctl(m_epfd, op, fd, &event);
        assert(rt == 0);
      } else {
        int op = EPOLL_CTL_ADD;
        int rt = epoll_ctl(m_epfd, op, fd, &event);
        assert(rt == 0);
        m_fds.emplace_back(fd);
      }
      return;
  }
  void delEventInLoopThread(int fd) {
    std::lock_guard<std::mutex> lock(m_mtx);
      auto it = find(m_fds.begin(), m_fds.end(), fd);
      if (it != m_fds.end()) {
        int op = EPOLL_CTL_DEL;
        int rt = epoll_ctl(m_epfd, op, fd, nullptr);
        assert(rt == 0);
      }
      return;
  }

 private:
  // -------- 核心内核资源 --------
  int m_epfd     {-1};    // epoll 实例
  int m_wake_fd  {-1};    // eventfd，用于跨线程唤醒 loop
  int m_timer_fd {-1};    // timerfd，用于定时器
  pid_t m_tid    {0};     // 本 Reactor 归属的线程 id

  // -------- 运行状态 --------
  bool m_stop_flag {false};
  bool m_is_looping {false};
  MyReactorType m_reactor_type {kSubReactor};

  // -------- 本地数据结构 --------
  // 已经注册到 m_epfd 的 fd 集合，用于析构时批量清理
  std::vector<int> m_fds;
  std::atomic<int> m_fd_size {0};

  // 延迟处理队列：跨线程调 addEvent/delEvent 会先堆在这，
  // 由 loop 线程轮到 C 阶段前统一消化
  std::map<int, epoll_event>              m_pending_add_fds;
  std::vector<int>                        m_pending_del_fds;
  std::vector<std::function<void()>>      m_pending_tasks;

  // TODO: 自行加一个 Mutex 保护上面三个 pending 容器
  // tinyrpc::Mutex m_mutex;
  std::mutex m_mtx;
  tinyrpc::Timer* m_timer {nullptr};
};


// -----------------------------------------------------------------------------
// MyReactor::loop() 骨架（只有阶段注释，实现自己填）
// -----------------------------------------------------------------------------
//
// 以下是"五拍循环"的伪代码模板：
//
/*
void MyReactor::loop() {
  // 守门：只允许本线程进入
  // TODO: assert(isLoopThread());
  // TODO: 防重入，设置 m_is_looping = true
  m_stop_flag = false;

  tinyrpc::Coroutine* first_coroutine = nullptr;
  // first_coroutine 是 loop 局部变量（不是成员！），意义：
  //   "上一轮 epoll_wait 登记的第一个就绪协程，本轮开头 Resume"
  // 保持它是局部变量的好处：它的生命周期仅在 loop 内部，不会被别的线程看到。

  while (!m_stop_flag) {
    epoll_event re_events[MAX_EVENTS + 1];

    // =======================================================================
    // 阶段 A：Resume 上一轮登记的协程（本线程预约槽）
    // =======================================================================
    // TODO:
    //   if (first_coroutine) {
    //     Coroutine::Resume(first_coroutine);
    //     first_coroutine = nullptr;
    //   }
    //
    // 问自己：
    //   · 为什么 first_coroutine 的 Resume 要放在下一圈开头，而不是上一圈 E 阶段？
    //     （提示：E 阶段还在遍历 re_events，Resume 进去子协程里 yield 回来时，
    //      re_events 还没处理完）

    // =======================================================================
    // 阶段 B：从全局队列偷活（仅 SubReactor）
    // =======================================================================
    // TODO:
    //   if (m_reactor_type != kMainReactor) {
    //     while (true) {
    //       FdEvent* ptr = GlobalCoroQueue::GetInstance()->pop();
    //       if (!ptr) break;
    //       ptr->setReactor(this);                   // 重新绑定所有权
    //       Coroutine::Resume(ptr->getCoroutine());
    //     }
    //   }
    //
    // 问自己：
    //   · 为什么 MainReactor 不参与抢？
    //   · pop 出来后为什么要 setReactor(this)？不设会出什么问题？
    //   · 如果 Resume(ptr->getCoroutine()) 之后这个协程又 yield 到一个
    //     新的 fd 上，那个 fd 的 epoll_ctl(ADD) 会加到哪个 Reactor 的 m_epfd？

    // =======================================================================
    // 阶段 C：执行本地 pending_tasks + 处理 pending_add/del_fds
    // =======================================================================
    // TODO:
    //   先把 m_pending_add_fds / m_pending_del_fds 批量应用到 m_epfd
    //   再 swap 出 m_pending_tasks 在锁外执行（避免长时间持锁）
    //
    // 问自己：
    //   · 为什么 swap 出来再执行？如果在锁内直接遍历执行会怎样？
    //     （提示：task 内部可能又调 addTask，同一个锁就死锁了）

    // =======================================================================
    // 阶段 D：epoll_wait —— 本循环唯一的内核 block 点
    // =======================================================================
    // TODO:
    //   int rt = epoll_wait(m_epfd, re_events, MAX_EVENTS, t_max_epoll_timeout);
    //   if (rt < 0) { log error; continue; }
    //
    // 问自己：
    //   · t_max_epoll_timeout 应该设多长？长了/短了各有什么代价？
    //   · epoll_wait 返回时，re_events 数组里的顺序是不是按 fd 就绪先后排的？
    //     （答案：不保证。内核按就绪链表顺序返回，可能每次不同）

    // =======================================================================
    // 阶段 E：分发 re_events（一个都不 Resume，只登记/push）
    // =======================================================================
    // TODO:
    //   for (int i = 0; i < rt; ++i) {
    //     epoll_event one = re_events[i];
    //
    //     // E.1 唤醒 fd：读空 m_wake_fd 的 eventfd 内容
    //     if (one.data.fd == m_wake_fd) {
    //       drain_eventfd(m_wake_fd);
    //       continue;
    //     }
    //
    //     // E.2 一般 fd：从 one.data.ptr 拿到 FdEvent*
    //     FdEvent* ptr = (FdEvent*)one.data.ptr;
    //     if (!ptr) continue;
    //
    //     // E.3 错误事件检测
    //     if (!(one.events & EPOLLIN) && !(one.events & EPOLLOUT)) {
    //       delEventInLoopThread(ptr->getFd());
    //       continue;
    //     }
    //
    //     // E.4 协程路径 vs 回调路径
    //     if (ptr->getCoroutine()) {
    //       // 第一个留本地（省锁 + 局部性）
    //       if (!first_coroutine) {
    //         first_coroutine = ptr->getCoroutine();
    //         continue;
    //       }
    //       // 其余：SubReactor 扔全局，MainReactor 当场 Resume
    //       if (m_reactor_type == kSubReactor) {
    //         delEventInLoopThread(ptr->getFd());   // 所有权迁移必须摘！
    //         ptr->setReactor(nullptr);
    //         GlobalCoroQueue::GetInstance()->push(ptr);
    //       } else {
    //         Coroutine::Resume(ptr->getCoroutine());
    //         if (first_coroutine) first_coroutine = nullptr;  // 防双 Resume
    //       }
    //     } else {
    //       // 没绑协程的 fd：timer_fd / 自定义回调
    //       auto read_cb  = ptr->getCallBack(READ);
    //       auto write_cb = ptr->getCallBack(WRITE);
    //       if (ptr->getFd() == m_timer_fd) {
    //         read_cb();   // 定时器回调直接在 loop 线程执行
    //         continue;
    //       }
    //       // 其他纯回调 fd：包成 task 丢到 m_pending_tasks，下一轮 C 阶段跑
    //       if ((one.events & EPOLLIN) && read_cb)  addTask(read_cb, false);
    //       if ((one.events & EPOLLOUT) && write_cb) addTask(write_cb, false);
    //     }
    //   }
    //
    // 问自己：
    //   · E.4 里第一个留本地，如果这里强行改成"也扔全局队列"，
    //     单 SubReactor 场景会退化成什么行为？是否会死锁？
    //   · 如果同一个 fd 同时触发 EPOLLIN 和 EPOLLOUT，
    //     会走协程路径还是回调路径？（取决于 getCoroutine() 是否非空，
    //     一旦绑协程就一条路走到底）
  }

  m_is_looping = false;
}
*/


}  // namespace myrpc

#endif  // MY_REACTOR_SKELETON_H
