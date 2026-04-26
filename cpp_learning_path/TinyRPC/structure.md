# TinyRPC 项目全局架构

## 一、30 秒电梯描述

> TinyRPC 是一个基于 **有栈协程 + 主从 Reactor + epoll ET** 的 C++ RPC 框架，协议层用 TinyPb（自定义的 protobuf 封包格式），实现了 **同步风格代码 + 非阻塞 IO** 的网络模型。核心思路：让业务代码写起来像阻塞调用，底层通过协程 hook（`read/write/accept/connect/sleep`）自动在 EAGAIN 时 Yield，epoll 就绪再 Resume，避免回调地狱的同时享受高并发。

## 二、组件分层

```
┌──────────────────────────────────────────────────────────┐
│ Layer 1  应用层（用户业务）                                │
│   ├── 用户定义的 protobuf service (.proto)                 │
│   └── 继承实现 service 的 C++ 类（由 dispatcher 调用）      │
├──────────────────────────────────────────────────────────┤
│ Layer 2  RPC 协议层                                        │
│   ├── TinyPbCodeC     编解码（PB_START/END + 6 个 int32）  │
│   ├── TinyPbRpcDispacther  路由 service.method → 执行      │
│   ├── TinyPbRpcController  单次调用上下文                   │
│   └── TinyPbRpcClosure     回调 closure                     │
├──────────────────────────────────────────────────────────┤
│ Layer 3  连接层                                             │
│   ├── TcpServer       持有 acceptor + io_pool + dispatcher  │
│   ├── TcpAcceptor     listenfd + accept_cor                 │
│   ├── TcpConnection   connfd + read/write buf + loop_cor    │
│   ├── TcpBuffer       应用层环形缓冲                         │
│   └── TcpTimeWheel    连接空闲超时（时间轮）                  │
├──────────────────────────────────────────────────────────┤
│ Layer 4  网络事件层                                          │
│   ├── Reactor         epoll_fd + timerfd + eventfd + loop() │
│   ├── FdEvent         fd + listen_events + 挂协程           │
│   ├── IOThread        pthread + 一个 SubReactor             │
│   ├── IOThreadPool    N 个 IOThread + RR 分配                │
│   └── Timer           基于 timerfd 的精确定时器              │
├──────────────────────────────────────────────────────────┤
│ Layer 5  协程运行时                                          │
│   ├── Coroutine       coctx + 栈 + callback                  │
│   ├── CoroutinePool   协程对象池（栈复用）                    │
│   ├── coctx_swap.S    汇编保存/恢复寄存器                     │
│   └── coroutine_hook  accept/read/write/connect/sleep hook   │
├──────────────────────────────────────────────────────────┤
│ Layer 6  基础设施                                             │
│   ├── Config / Log / Mutex / RWMutex / ThreadPool             │
│   ├── MsgReqUtil  生成全局唯一 msg_req（雪花或类似）            │
│   └── RunTime     per-coroutine 请求上下文                     │
└──────────────────────────────────────────────────────────┘
```

## 三、线程模型

```
┌─────────────────────────────────────────────────────────┐
│  Main Thread（主线程）                                     │
│    ├── m_main_reactor  (MainReactor 类型)                 │
│    ├── m_accept_cor    跑 MainAcceptCorFunc               │
│    └── ClearClientTimerFunc  每 10s 清理 Closed 连接       │
│                                                           │
│        epoll 监听: listenfd + eventfd + timerfd            │
└─────────────────────────────────────────────────────────┘
                            │
                新连接 RR 分配到一个 IOThread
                            ▼
┌─────────────────────────────────────────────────────────┐
│  IOThread[0..N-1]  每个线程一个 SubReactor                 │
│    ├── m_reactor       (SubReactor 类型)                  │
│    ├── 管着若干 TcpConnection 的 loop_cor                 │
│    │                                                       │
│    │   epoll 监听: eventfd + timerfd + 本线程所有 connfd   │
│    │                                                       │
│    └── Reactor::loop() 的工作流:                           │
│        1. resume first_coroutine (上一轮登记)              │
│        2. 从全局 CoroutineTaskQueue pop 剩余协程继续 resume │
│        3. 执行 m_pending_tasks 里的 lambda                 │
│        4. epoll_wait(...)                                  │
│        5. 遍历事件:                                         │
│             - 第一个协程事件 → first_coroutine             │
│             - 其余 → 摘 fd + push 全局 CoroutineTaskQueue  │
│             - 纯 callback → push m_pending_tasks           │
│        6. 处理 pending_add/del_fds                         │
│        7. goto 1                                           │
└─────────────────────────────────────────────────────────┘
```

## 四、核心数据结构关系

```
TcpServer
├── m_main_reactor       : Reactor*   主 Reactor
├── m_accept_cor         : Coroutine::ptr
├── m_acceptor           : TcpAcceptor (listenfd)
├── m_io_pool            : IOThreadPool
├── m_codec              : AbstractCodeC  全局一份（无状态可共享）
├── m_dispatcher         : AbstractDispatcher  全局一份
├── m_clients            : map<fd, TcpConnection::ptr>  主线程独占
├── m_time_wheel         : TcpTimeWheel
└── m_clear_clent_timer_event : TimerEvent

IOThread
├── m_thread             : pthread_t
├── m_reactor            : Reactor*  (thread_local)
├── m_init_semaphore     : 初始化同步
├── m_start_semaphore    : 统一启动
└── m_index              : 池内下标

Reactor
├── m_epfd               : int    epoll_fd
├── m_wake_fd            : int    eventfd 跨线程唤醒
├── m_timer_fd           : int    timerfd 定时器
├── m_pending_tasks      : vector<function<void()>>  本线程任务队列
├── m_pending_add/del_fds: 待注册/注销的 fd
└── m_timer              : Timer*

TcpConnection
├── m_fd                 : int    connfd
├── m_fd_event           : FdEvent::ptr  挂 loop_cor
├── m_reactor            : Reactor*  所属 IO 线程的 Reactor
├── m_io_thread          : IOThread*
├── m_codec              : AbstractCodeC::ptr  指向 server 的 codec
├── m_read_buffer        : TcpBuffer::ptr
├── m_write_buffer       : TcpBuffer::ptr
├── m_loop_cor           : Coroutine::ptr  跑 MainServerLoopCorFunc
├── m_state              : NotConnected/Connected/HalfClosing/Closed
├── m_weak_slot          : 时间轮槽位（弱引用）
└── m_reply_datas        : map<msg_req, pb_ptr> （仅客户端用）

FdEvent
├── m_fd                 : int
├── m_reactor            : Reactor*
├── m_listen_events      : READ | WRITE
└── m_coroutine          : Coroutine*  挂在这个 fd 上的协程

Coroutine
├── m_cor_id             : int    (主协程 0，子协程自增)
├── m_stack_sp / m_stack_size : 栈指针和大小
├── m_coctx              : 寄存器 + RSP + RIP
├── m_call_back          : function<void()>  用户逻辑
├── m_can_resume         : bool   幂等保护
└── m_run_time           : RunTime  请求上下文
```

## 五、完整生命周期（一次 RPC 调用）

```
T0  服务启动
    Main: TcpServer 构造 → acceptor.init (listen)
          + IOThreadPool 构造（每个 IOThread pthread_create）
          + 挂 ClearClientTimerFunc（10s 周期）

T1  Main: accept_cor.Resume → MainAcceptCorFunc
          → accept_hook: 先 try accept，EAGAIN 就 toEpoll(READ) + Yield
          → Main Reactor 进入 epoll_wait 等 listenfd

T2  Client connect → 3WHS → listenfd 可读
    Main Reactor: epoll_wait 返回 → Resume accept_cor
                → accept_hook 内 g_sys_accept_fun 返回 connfd
                → 继续 MainAcceptCorFunc while 循环
                → io_pool.getIOThread() 选一个 IO 线程 (RR)
                → new TcpConnection + initServer
                → io_thread->reactor->addCoroutine(loop_cor)
                  [内部 eventfd_write 唤醒 IO 线程]

T3  IO 线程 epoll_wait 醒 (因 eventfd) → 执行 pending_tasks
    Resume(loop_cor) → 第一次跑 MainServerLoopCorFunc
    → input(): read_hook → EAGAIN → toEpoll(READ) + Yield
    → IO 线程回主协程 → epoll_wait 等 connfd

T4  Client send req → connfd 可读
    IO 线程 epoll_wait 返回 → 找 fd_event.coroutine → Resume(loop_cor)
    → loop_cor 回到 read_hook Yield 后那行 → 真 read
    → input 完成
    → execute(): codec->decode → 切出完整包 → dispatcher->dispatch
      → dispatch 找 service/method → ParseFromString → CallMethod (⚠️ 用户同步业务)
      → response→SerializeToString → codec->encode 写 write_buffer
    → output(): write_hook → 若 EAGAIN → toEpoll(WRITE) + Yield
    → [WRITE 就绪] resume → 真 write 到 socket
    → while 回到 input() → 再 EAGAIN → Yield

T5  Client close → FIN → connfd EPOLLIN
    → Resume(loop_cor) → read 返回 0 → close_flag=true
    → clearClient: fd_event.unregister + close(fd) + setState(Closed)
    → setCanResume(false) + Yield (永不再 resume)

T6  Main 线程 10s 后 ClearClientTimerFunc 扫 m_clients
    → 发现 state==Closed → m_clients[fd].reset()
    → ~TcpConnection → returnCoroutine(loop_cor) 归还协程池
```

## 六、关键设计决策（面试 talking points）

### 6.1 为什么主从 Reactor 分开？
- **职责分离**：accept 是 CPU 极轻但频率高的操作，不能被 read/write 干扰；IO 是吞吐大户，不能被 accept 抢占
- **可扩展**：Main 只一个线程够用（accept 不可能 accept 几十万 QPS），IO 随核数 N 扩展
- **惊群**：如果多个线程 accept 同一个 listenfd，会有惊群问题；单主线程 accept 规避

### 6.2 为什么 connection 绑定 IO 线程而不是每个请求调度？
- **buffer 无锁**：read_buffer/write_buffer 只被一个线程访问，不需要锁
- **协议状态无锁**：decode 的解析状态（半包、粘包上下文）不用跨线程同步
- **cache 友好**：同连接频繁访问的 buffer/fd_event 在一个 CPU 的 L1/L2

### 6.3 为什么有全局 CoroutineTaskQueue？
- 缓解 RR 分配不均（某个 IO 线程 epoll_wait 返回一批事件时，只 resume 第一个，其余让其他空闲线程抢）
- **代价**：打破线程亲和 + 全局锁竞争 + 第一个慢协程仍然堵本线程。这是个折中，不是最优解

### 6.4 为什么要 hook read/write/accept/connect/sleep？
- 这 5 个 syscall 都会阻塞线程
- hook 后在 EAGAIN/EINPROGRESS 时自动 Yield，epoll 就绪再 Resume
- 用户业务代码写同步风格，底层自动非阻塞

### 6.5 TinyPb 协议为什么不用 HTTP/gRPC？
- TinyPb 是自研二进制协议：`PB_START | pk_len | msg_req_len | msg_req | service_name_len | service_name | err_code | err_info_len | err_info | pb_data | checksum | PB_END`
- 优点：比 HTTP 文本头轻、比 gRPC HTTP/2 帧简单、教学友好
- 缺点：没有流式、没有头部压缩、没有多路复用、没有生态（debug 工具/代理/监控都没有现成的）

## 七、已知问题 & 改造方向（简历亮点）

| # | 问题 | 代码位置 | 改造方向 |
|---|---|---|---|
| 1 | `decode` 里 `vector<char> tmp = buf->getBufferVector()` 按值拷贝整个 buffer | `tinypb_codec.cc:160` | 直接用裸指针 + readIndex，省一次大 memcpy |
| 2 | IO 业务耦合，慢 handler 拖垮整个 IO 线程 | `tinypb_rpc_dispatcher.cc:111` | 拆业务线程池 + lambda 回送 + per-IO pending queue |
| 3 | read/write hook 无超时 | `coroutine_hook.cc:read_hook/write_hook` | 加 TimerEvent 挂 timerfd，超时 setErrno=ETIMEDOUT |
| 4 | 无全链路 try/catch，业务 throw 能挂进程 | `MainServerLoopCorFunc / dispatch` | 外层 try/catch(...) → 转 error response |
| 5 | 连接清理靠 10s 定时器扫 | `tcp_server.cc:249 ClearClientTimerFunc` | Closed 后立即投递清理任务，不等扫描 |
| 6 | 全局 CoroutineTaskQueue 单锁 | `reactor.cc:437-450` | per-thread queue + work stealing |
| 7 | `encode` 内先 malloc 再 memcpy 再 free | `tinypb_codec.cc:78` | 直接在 write_buffer 上 reserve + 填充 |
| 8 | protobuf request/response 用裸 new/delete | `tinypb_rpc_dispatcher.cc:81,99,126-127` | 用 unique_ptr 或 arena allocator |

## 八、如何向面试官讲这个项目（3 分钟版）

> "我基于 Formulka 的 TinyRPC 开源框架做了深度理解和部分改造。核心架构是有栈协程 + 主从 Reactor 的 RPC 框架，协议层自研 TinyPb 封包。
>
> **我关注的是三件事**：
>
> **第一**，我完整梳理了一次 RPC 调用的生命周期——从 Main Reactor 的 accept 协程，到新连接 RR 分配给 IO 线程、loop_cor 通过 hook 的 read/write 自动 Yield/Resume，再到 dispatcher 路由业务方法、encode/write 回客户端，每一步都能追到代码行。
>
> **第二**，我识别了架构里几个典型问题：最严重的是 **IO 业务耦合**，业务 handler 同步跑在 IO 线程协程栈上，一个慢查询会拖垮同线程几百个连接的 P99。这是协作式协程的根本限制——业务不 Yield，epoll_wait 就回不来，连 timerfd 超时都失效。
>
> **第三**，我设计了改造方案：IO 线程只做收发包和 dispatch 查表，业务 handler 扔到独立 worker 线程池，跨线程用 eventfd + per-IO pending queue 做异步回送，worker 完成后打包 lambda 丢给原 IO 线程 encode + 注册 EPOLLOUT。代价是每请求多一两次 eventfd 写入和一次锁，但换来 P99 稳定性——这就是 brpc bthread 的思路。
>
> 时间原因我没完全落地这个改造，但压测数据[如果你补了]、decode 里 vector 拷贝的优化[如果你改了]等小改动已经做完。"

