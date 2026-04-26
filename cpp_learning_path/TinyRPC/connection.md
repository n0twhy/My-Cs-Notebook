## Connection 业务与IO
连接 conn 被分到 IO_A → 它的 loop_cor 被投递到 IO_A 等待第一次执行 → 第一次 Resume 时 loop_cor 跑 input() 碰到 EAGAIN 就订阅 connfd 读事件并 Yield → 之后每次 connfd 有读/写事件，IO_A 的 epoll_wait 返回，从 fd_event 找到 loop_cor 再 Resume，继续 input/execute/output 流程。IO_A 同时管着很多个这样的连接，谁的 fd 有事件就 resume 谁。

# handler同步跑在IO线程里，如果其中一个是慢查询会拖累这个线程的所有连接，坑非常多
发现协作式协程的根本限制：
   → 业务不 Yield → IO 线程回不到 epoll_wait
   → read timeout 也触发不了 (因为 timer 靠 epoll 驱动)
   → 最终结论：必须把业务移出 IO 线程
设计新架构：IO 和业务分离
   → 但又引出新问题：线程间通信怎么搞？
   → 讨论了 cv/sem (❌ 会让 IO 线程睡) vs eventfd+queue (✅ IO 线程永不睡)
   → 讨论了全局队列 (❌ 锁竞争) vs per-IO-thread queue (✅)
设计数据回传路径：
   → response 不能让 worker 直接塞 write_buffer (线程不安全)
   → 打包成 lambda 丢给 IO 线程 pending tasks
   → lambda 做"encode + 注册 EPOLLOUT"
   → loop_cor 的 output() 做"真正 write syscall"
   → 两者接力，不重复
# read和write，没有设置超时，慢攻击会打爆协程池
读写如果碰到eagain就会挂起，没有挂任何的超时solution。
可以在挂起的时候做一个timer定时器，如果到时间，就回来释放整个协程。
# 异常安全没做 + 连接清理靠 10s 定时器
全链路 try/catch，异常转 error response

# 新架构
TinyRPC 原本的架构让 IO 和业务跑在同一个协程、同一个线程上。这在协程框架里其实是个常见陷阱：协程是协作式调度的，只有主动 Yield 才能让出 CPU。如果业务 handler 里有同步 MySQL 查询、大循环计算这种不会触发 Yield 的代码，整个 IO 线程的 epoll_wait 就回不来，表现就是队头阻塞，P99 延迟直接飙到业务耗时级别。更严重的是，因为 epoll_wait 回不来，基于 timerfd 的 read/write 超时也失效

我的优化方向是 IO 和业务分离：IO 线程只做收发包 + 协议编解码 + dispatch 查表，业务 handler 移到独立的 worker 线程池。跨线程通信用 eventfd + per-IO-thread pending queue 做异步回送，worker 完成后不直接碰连接的 write_buffer，而是打包一个 lambda 闭包丢给 IO 线程，lambda 在 IO 线程上做 encode + 注册 EPOLLOUT，真正的 write 仍然走 loop_cor 的 output。这样既保证了 IO 路径的线程安全（write_buffer 依然无锁单线程访问），又让慢 handler 不会波及 IO。

┌────────────────────────────────────────────────────────────────────┐
│                       新架构一览                                     │
│                                                                     │
│  Client                                                             │
│    │                                                                │
│    ▼                                                                │
│  [Main Reactor] ─── accept_cor ─── 分配新连接                       │
│    │  (1 个线程)                                                    │
│    ▼                                                                │
│  [IO Thread Pool] ← N 个线程，每个一个 SubReactor                   │
│    │   职责：read/write、decode/encode、dispatch 查表               │
│    │   每连接一个 loop_cor，仍跑 input/execute/output               │
│    │                                                                │
│    │   decode 完成后，执行 execute:                                 │
│    │     ┌─ 不直接调 CallMethod                                     │
│    │     └─ 打包 Task{handler, req, conn_weak, io_reactor}          │
│    │        push 到 Business Queue                                  │
│    │        eventfd_write 唤醒业务线程池                            │
│    │     立刻返回 → output() 继续处理 write_buffer 里已有的数据     │
│    │     → Yield 回 epoll_wait                                      │
│    ▼                                                                │
│  [Business Thread Pool] ← M 个线程，每个有本地任务队列               │
│    │   职责：跑 user handler (CallMethod)                           │
│    │                                                                │
│    │   完成后:                                                       │
│    │     1. response → SerializeToString                            │
│    │     2. 打包 lambda: [conn_weak, reply_pb_ptr]() {              │
│    │          encode 到 write_buffer                                │
│    │          注册 EPOLLOUT                                         │
│    │        }                                                        │
│    │     3. push lambda 到 "conn 所属 IO 线程" 的 pending_tasks     │
│    │     4. eventfd_write 唤醒该 IO 线程                            │
│    │                                                                │
│    ▼                                                                │
│  [IO 线程被唤醒] → 执行 pending_tasks 里的 lambda                   │
│    → encode 完成、EPOLLOUT 注册                                     │
│    → 等内核 socket 可写 → EPOLLOUT → resume loop_cor               │
│    → output() → write syscall → 数据发出                            │
│                                                                     │
└────────────────────────────────────────────────────────────────────┘

代价是多了一次eventfd写入，然后有单独一个IO线程内部的锁竞争，但是换来的就是P99稳定性
如果原来一个mysql的慢查询，handler拖了整整200ms(因为他并不是阻塞，而是单纯的执行慢，但是都造成了同样的后果)，那么这个连接所有的任务，都得等这个200ms。新架构开了一个worker线程，只占用这一个，而原IO线程decode完，拿到具体method分发出去异步等任务执行完把执行的lambda(做encode编码送到原IO线程的task队列)作为task送给原IO线程reactor的epoll上面，epoll_wait读到就完事儿了，完全不会因为任务执行慢就拖垮整个IO线程的连接