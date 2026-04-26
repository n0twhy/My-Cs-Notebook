// =============================================================================
// ET 单线程 Reactor Echo Server —— 骨架版（函数体全是 TODO，自己填）
//
// 目标：吃透 4 件事
//   1. ET 为什么必须非阻塞 + while 到 EAGAIN
//   2. EPOLLOUT 的注册 / 取消时机
//   3. fd 生命周期：accept 到 close 经过哪些状态
//   4. 错误处理：EAGAIN / EINTR / ECONNRESET / EPIPE 各自含义
//
// 填写顺序建议（按难度 & 依赖）：
//   🔥1  setNonBlocking         最简单，先热身
//   🔥2  createListenSocket     socket/bind/listen
//   🔥3  main 里的事件循环      epoll_create1 + epoll_wait 空转先跑通
//   🔥4  handleAccept           ET while accept 到 EAGAIN
//   🔥5  handleRead             ET while read 到 EAGAIN + echo 回写第一把
//   🔥6  handleWrite            EPOLLOUT 回调，发 out_buf 剩余
//   🔥7  closeConnection        epoll_ctl(DEL) + close + 回收 Connection
//
// 不允许引入：
//   · 协程（ucontext / boost.coroutine）—— 下一阶段再说
//   · 多线程 —— 单线程 Reactor
//   · 第三方库（除 STL 外）
// =============================================================================


#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// -----------------------------------------------------------------------------
// 全局常量
// -----------------------------------------------------------------------------
static constexpr int    kPort         = 8080;
static constexpr int    kBacklog      = 1024;   // listen() 的 backlog，对应 accept 队列上限
static constexpr int    kMaxEvents    = 1024;   // 一次 epoll_wait 最多取多少事件
static constexpr size_t kReadChunk    = 4096;   // 每次 read 的栈上缓冲大小
static constexpr int    kEpollTimeout = -1;     // -1 = 永久阻塞直到有事件；练习里先这样

// -----------------------------------------------------------------------------
// Connection：一个 clientfd 的所有状态
// -----------------------------------------------------------------------------
//
// 🔥 思考：为什么需要这些字段？
//   · fd        —— epoll 事件回来时 O(1) 找到自己
//   · out_buf   —— 短写/EAGAIN 时把没发完的字节留这，下次 EPOLLOUT 继续发
//   · out_sent  —— out_buf 里已经发出去的字节偏移（也可以直接 erase 头部，看你怎么写）
//   · writing   —— 当前是否已经注册了 EPOLLOUT，用来避免重复 MOD
//   · peer_closed —— read 返回 0 后置 true，等 out_buf 发空就 close
//
// 🔥 思考：为什么 in_buf 不单列？echo 场景可以不保留，读到就直接追加到 out_buf 准备回写。
//   真实业务里（HTTP/RPC）需要 in_buf 做半包/粘包拼接，那是下一阶段的事。
// -----------------------------------------------------------------------------
class Connection {
 public:
    explicit Connection(int fd) : fd_(fd) {}
    ~Connection() = default;

    // 禁拷贝：一个连接只能被一个 owner 持有（对应 unique_ptr 的语义）
    // 🔥 八股触发：为什么 Reactor 里的连接对象要禁拷贝？能默认移动吗？
    Connection(const Connection&)            = delete;
    Connection& operator=(const Connection&) = delete;

    // -------- 只读访问 --------
    int  fd()           const { return fd_; }
    bool writing()      const { return writing_; }
    bool peer_closed()  const { return peer_closed_; }
    bool out_buf_empty() const { return out_sent_ >= out_buf_.size(); }

    // -------- 读路径接口 --------
    // ET 下 while 到 EAGAIN 读。返回值：
    //    0  正常结束（EAGAIN）
    //   >0  对端 FIN（设置 peer_closed_ = true）
    //   -1  致命错误，需要 close
    // 读到的字节直接 append 进 out_buf_（echo 语义）。
    int readAll();

    // -------- 写路径接口 --------
    // 把 out_buf_[out_sent_..] 尽量写出去。返回值：
    //    0  out_buf 发空
    //    1  发了一部分遇到 EAGAIN，需要等 EPOLLOUT
    //   -1  致命错误（EPIPE/ECONNRESET）
    int flush();

    // 外部主动追加待发数据（非 echo 场景备用）
    void appendOut(const char* data, size_t n) { out_buf_.append(data, n); }

    // EPOLLOUT 状态位
    void markWriting(bool v) { writing_ = v; }

 private:
    int         fd_          {-1};
    std::string out_buf_;              // 待发缓冲（未发完的尾巴）
    size_t      out_sent_    {0};      // out_buf 中已发送偏移
    bool        writing_     {false};  // 是否已向 epoll 注册 EPOLLOUT
    bool        peer_closed_ {false};  // 收到对端 FIN（read 返回 0）
};

// fd -> Connection 的全局表。close 后必须 erase，否则内存泄漏。
// 用裸指针或 unique_ptr 均可；面试常问：为什么不用 shared_ptr？（引用计数开销 + 所有权不清）
static std::unordered_map<int, std::unique_ptr<Connection>> g_conns;

// 全局 epoll fd，偷懒放全局；生产代码应封进一个 Reactor 类
static int g_epfd = -1;

// -----------------------------------------------------------------------------
// 工具层
// -----------------------------------------------------------------------------

// 🔥1 把 fd 设为 O_NONBLOCK
//    参考：fcntl(fd, F_GETFL) → | O_NONBLOCK → fcntl(F_SETFL)
//    注意：accept4 / socket 可以直接传 SOCK_NONBLOCK 避免这一步，但保留这个工具以备不时
static int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    // TODO:
    //   1. int flags = fcntl(fd, F_GETFL, 0);
    //   2. if (flags == -1) return -1;
    //   3. return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return -1;
}

// 🔥2 创建 listen socket：socket → setsockopt(SO_REUSEADDR) → bind → listen → 非阻塞
//    面试高频：SO_REUSEADDR 和 SO_REUSEPORT 的区别？为什么 server 重启后 TIME_WAIT 占端口要它？
static int createListenSocket(uint16_t port) {
  int lfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
  int opt = 1;
  setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
  listen(lfd, kBacklog); //表示全连接队伍的最大长度
  return lfd;

    // TODO:
    //   int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    //   int opt = 1; setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //   sockaddr_in addr{}; addr.sin_family = AF_INET;
    //                       addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //                       addr.sin_port = htons(port);
    //   bind(lfd, ...);
    //   listen(lfd, kBacklog);
    //   return lfd;
}

// epoll 操作的小包装：注册 / 修改 / 删除。
// events 典型值：EPOLLIN | EPOLLET，或 EPOLLIN | EPOLLOUT | EPOLLET
static int epollAdd(int fd, uint32_t events) {
    // TODO: epoll_event ev{}; ev.events = events; ev.data.fd = fd;
    //       return epoll_ctl(g_epfd, EPOLL_CTL_ADD, fd, &ev);
    epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    int rt = epoll_ctl(g_epfd, EPOLL_CTL_ADD, fd, (struct epoll_event*)&ev);
    return rt;
}
static int epollMod(int fd, uint32_t events) {
    // TODO: 同上，op = EPOLL_CTL_MOD
    epoll_event cv;
    cv.events = events;
    cv.data.fd = fd;
    int rt = epoll_ctl(g_epfd, EPOLL_CTL_MOD, fd, (struct epoll_event*)&cv);
    return rt;
}
static int epollDel(int fd) {
    // TODO: epoll_ctl(g_epfd, EPOLL_CTL_DEL, fd, nullptr)
    //   注意：close(fd) 之后内核会自动从 epoll 摘，但：
    //     1. fd 被 dup 过 / 被其他 epoll 监听时行为复杂；
    //     2. 显式 DEL 是面试官眼里的"好习惯"，别省。
    int rt = epoll_ctl(g_epfd, EPOLL_CTL_DEL, fd, nullptr);
    return rt;
}

// -----------------------------------------------------------------------------
// 业务层
// -----------------------------------------------------------------------------

// 🔥7 关闭一个连接的完整流程
//    调用时机：read == 0 且 out_buf 发空 / ECONNRESET / EPIPE / 业务主动踢
static void closeConnection(int fd) {
  auto it = g_conns.find(fd);
  if (it == g_conns.end()) return;
  
  epollDel(fd);
  g_conns.erase(it);
  close(fd);
    // TODO:
    //   1. epollDel(fd)                          —— 先摘出 epoll
    //   2. close(fd)                              —— 再 close 内核 fd
    //   3. g_conns.erase(fd)                      —— 回收 Connection 对象
    //
    // 思考题（面试高频）：
    //   · 如果先 close(fd) 再 epollDel，会出什么问题？
    //     （提示：fd 号可能立刻被新 accept 复用，epollDel 就删错连接了）
}

// 🔥4 handleAccept —— listenfd 可读
//    核心纪律：ET 下必须 while 抽干 accept 队列
static void handleAccept(int listenfd) {
  while (true) {
    sockaddr_in peer {};
    socklen_t len = sizeof(peer);
    int cfd = accept4(listenfd, (struct sockaddr *)&peer, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);

    if (cfd >= 0) {
      auto conn = std::make_unique<Connection>(cfd);
      g_conns[cfd] = std::move(conn);
      epollAdd(cfd, EPOLLIN | EPOLLET);
      continue;
    }
    if (errno == EINTR) continue;
    if (errno == EAGAIN) break;
    if (errno == ECONNABORTED) continue;
    perror("accept4");
    break;
  }
    // TODO:
    //   while (true) {
    //     sockaddr_in peer{}; socklen_t len = sizeof(peer);
    //     int cfd = accept4(listenfd, (sockaddr*)&peer, &len,
    //                       SOCK_NONBLOCK | SOCK_CLOEXEC);
    //     if (cfd >= 0) {
    //       // 可选：setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, ...)
    //       auto conn = std::make_unique<Connection>();
    //       conn->fd = cfd;
    //       g_conns[cfd] = std::move(conn);
    //       epollAdd(cfd, EPOLLIN | EPOLLET);     // ★ 只挂 EPOLLIN，不要挂 EPOLLOUT
    //       continue;
    //     }
    //     // cfd == -1
    //     if (errno == EINTR)                continue;
    //     if (errno == EAGAIN ||
    //         errno == EWOULDBLOCK)          break;   // ★ 队列真空了，退出
    //     if (errno == ECONNABORTED)         continue; // client 握手后立刻 RST，跳过
    //     perror("accept4"); break;
    //   }
}

// Connection::readAll 和 Connection::flush 的实现放这里（签名见类定义）
// 填的时候注意：所有 ::read / ::write 都要处理 EINTR / EAGAIN / ECONNRESET / EPIPE
int Connection::readAll() {
  while (true) {
    char buf[kReadChunk];
    ssize_t n = read(fd_, &buf[0], sizeof(buf));
    if (n > 0) {
      out_buf_.append(buf, n);
      continue;
    }
    if (n == 0) {
      peer_closed_ = true;
      return 1;
    }
    if (n < 0 && errno == EINTR) continue;
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return 0;
    if (n < 0 && errno == ECONNRESET) return -1;

    return -1;
  }
    // TODO:
    //   while (true) {
    //     char buf[kReadChunk];
    //     ssize_t n = ::read(fd_, buf, sizeof(buf));
    //     if (n > 0)  { out_buf_.append(buf, n); continue; }
    //     if (n == 0) { peer_closed_ = true; return 1; }          // 对端 FIN
    //     if (n < 0 && errno == EINTR)          continue;
    //     if (n < 0 && (errno == EAGAIN ||
    //                   errno == EWOULDBLOCK))  return 0;          // recv 队列抽干
    //     if (n < 0 && errno == ECONNRESET)     return -1;
    //     return -1;  // 其他未知错误
    //   }
    return -1;
}

int Connection::flush() {
  while (out_sent_ < out_buf_.size()) {
    ssize_t n = write(fd_, out_buf_.data() + out_sent_, out_buf_.size() - out_sent_);
    if (n > 0) {
      out_sent_ += n;
      continue;
    }
    if (n < 0 && errno == EINTR) continue;
    if (n < 0 && errno == EAGAIN) return 1;
    return -1;
  }
  out_buf_.clear();
  out_sent_ = 0;
  return 0;

    // TODO:
    //   while (out_sent_ < out_buf_.size()) {
    //     ssize_t n = ::write(fd_,
    //                         out_buf_.data() + out_sent_,
    //                         out_buf_.size() - out_sent_);
    //     if (n > 0) { out_sent_ += n; continue; }
    //     if (n < 0 && errno == EINTR)  continue;
    //     if (n < 0 && errno == EAGAIN) return 1;
    //     return -1;   // EPIPE / ECONNRESET / 其他
    //   }
    //   out_buf_.clear();
    //   out_sent_ = 0;
    //   return 0;
    return -1;
}

// 🔥5 handleRead —— clientfd EPOLLIN
//    核心纪律：
//    · ET 下必须 while read 到 EAGAIN
//    · read 返回 0 是对端 FIN，不是"读完"
//    · echo 语义：读到的字节追加到 out_buf，读循环结束后尝试 flushOutBuf 一次
static void handleRead(int fd) {
  auto it = g_conns.find(fd);
  if (it == g_conns.end()) return;

  Connection *c = it->second.get();
  int r = c->readAll();

  if (r == -1) {
    closeConnection(fd);
    return;
  }
  if (!c->out_buf_empty()) {
    int w = c->flush();
    if (w == -1) {
      closeConnection(fd);
      return;
    }
    if (w == 1) {
      if (!c->writing()) {
        epollMod(fd, EPOLLIN | EPOLLOUT | EPOLLET);
        c->markWriting(true);
      }
      return;
    }    
  }
  if (c->peer_closed() && c->out_buf_empty()) closeConnection(fd);
    // TODO:
    //   auto it = g_conns.find(fd); if (it == g_conns.end()) return;
    //   Connection* c = it->second.get();
    //
    //   int r = c->readAll();
    //   if (r == -1) { closeConnection(fd); return; }
    //
    //   // 读完后尝试立刻回写（echo 语义）
    //   if (!c->out_buf_empty()) {
    //     int w = c->flush();
    //     if (w == -1) { closeConnection(fd); return; }
    //     if (w ==  1) {                              // 还有没发完的
    //       if (!c->writing()) {
    //         epollMod(fd, EPOLLIN | EPOLLOUT | EPOLLET);
    //         c->markWriting(true);
    //       }
    //       return;
    //     }
    //     // w == 0：全发空了，什么都不用改
    //   }
    //
    //   // 对端已 FIN 且 out_buf 发空 —— 这才是真正 close 的时机
    //   if (c->peer_closed() && c->out_buf_empty()) closeConnection(fd);
}

// 🔥6 handleWrite —— clientfd EPOLLOUT
//    只做一件事：把 out_buf 剩下的发完，发空后摘掉 EPOLLOUT
static void handleWrite(int fd) {
  auto it = g_conns.find(fd);
  if (it == g_conns.end()) return;
  Connection *c = it->second.get();

  int r = c->flush();
  if (r == -1) {
    closeConnection(fd);
    return;
  }
  if (r == 1) return;

  epollMod(fd, EPOLLIN | EPOLLET);
  c->markWriting(false);
  if (c->peer_closed()) closeConnection(fd);

    // TODO:
    //   auto it = g_conns.find(fd); if (it == g_conns.end()) return;
    //   Connection* c = it->second.get();
    //
    //   int r = c->flush();
    //   if (r == -1) { closeConnection(fd); return; }
    //   if (r ==  1) return;   // 还没发完，继续等下一次 EPOLLOUT
    //
    //   // r == 0：out_buf 已发空 —— ★ 摘掉 EPOLLOUT（铁律）
    //   epollMod(fd, EPOLLIN | EPOLLET);
    //   c->markWriting(false);
    //
    //   // 如果对端已 FIN，等的就是这一刻
    //   if (c->peer_closed()) closeConnection(fd);
}

// -----------------------------------------------------------------------------
// 🔥3 main：事件循环
// -----------------------------------------------------------------------------
int main() {
    // 必须忽略 SIGPIPE，否则对端 RST 后继续 write 会让进程 SIGPIPE 退出
    signal(SIGPIPE, SIG_IGN);
    g_epfd = epoll_create1(EPOLL_CLOEXEC);
    int lfd = createListenSocket(kPort);
    epollAdd(lfd, EPOLLIN | EPOLLET);

    while (true) {
      epoll_event evs[kMaxEvents];
      int n = epoll_wait(g_epfd, (struct epoll_event *)&evs[0], kMaxEvents, kEpollTimeout);
      if(n < 0) {
        if (errno == EINTR) continue;
        perror("epoll_wait");
        break;
      }

      for (int i = 0; i < n; ++i) {
        epoll_event ep_event = evs[i];
        int fd = ep_event.data.fd;
        auto events = ep_event.events;
        
        if (fd == lfd) {
          handleAccept(lfd);
          continue;
        }
        if (events & (EPOLLERR | EPOLLHUP)) {
          closeConnection(fd);
          continue;
        }
        if (events & EPOLLIN) {
          handleRead(fd);
        }
        if (events & EPOLLOUT) {
          handleWrite(fd);
        }
        


      }
    }
    // TODO:
    //   1. g_epfd = epoll_create1(EPOLL_CLOEXEC);
    //   2. int lfd = createListenSocket(kPort);
    //      epollAdd(lfd, EPOLLIN | EPOLLET);   // listenfd 也用 ET
    //
    //   3. while (true) {
    //        epoll_event evs[kMaxEvents];
    //        int n = epoll_wait(g_epfd, evs, kMaxEvents, kEpollTimeout);
    //        if (n < 0) {
    //          if (errno == EINTR) continue;      // 被信号打断，继续
    //          perror("epoll_wait"); break;
    //        }
    //        for (int i = 0; i < n; ++i) {
    //          int       fd = evs[i].data.fd;
    //          uint32_t ev = evs[i].events;
    //
    //          if (fd == lfd) { handleAccept(lfd); continue; }
    //
    //          // 错误事件：EPOLLERR / EPOLLHUP 需要处理
    //          if (ev & (EPOLLERR | EPOLLHUP)) {
    //            closeConnection(fd); continue;
    //          }
    //          if (ev & EPOLLIN)  handleRead(fd);
    //          if (ev & EPOLLOUT) handleWrite(fd);
    //          //   ★ 思考：为什么 EPOLLIN 和 EPOLLOUT 都用 if，不是 else if？
    //          //     （同一次 epoll 返回可能两个都置位，必须都跑）
    //          //   ★ 思考：handleRead 里可能 closeConnection，handleWrite 再跑会怎样？
    //          //     （g_conns.find 会失败，handleWrite 里要做 guard）
    //        }
    //      }

    return 0;
}

// =============================================================================
// 填完之后，先拿 nc 测：
//   $ ./et_echo_server
//   $ nc 127.0.0.1 8080
//   输入任意文字回车，server 应原样回显。Ctrl+D 或 Ctrl+C 能干净断开。
//
// 然后再进 Step 3：我会提 5 个故障注入场景问你。
// =============================================================================
