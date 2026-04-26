# Coroutine 协程

## 一、协程本质：可暂停/恢复的函数

一个函数的执行状态由 5 样东西决定，只要能保存+恢复这 5 样，就能让函数"暂停"再"从当时那行继续"：
1. 通用寄存器（RAX / RBX / R12~R15 等）
2. RIP 指令指针（下一条要执行的指令地址）
3. RSP 栈指针
4. 整个栈的内容
5. CPU 标志寄存器（RFLAGS）

有栈协程就是把这些打包进一个 `coctx` 结构体，切换时做一次 `coctx_swap`（汇编写的）。

## 二、有栈 vs 无栈

| | 有栈协程 (Stackful) | 无栈协程 (Stackless) |
|---|---|---|
| 代表 | TinyRPC / libco / goroutine | C++20 `co_await` / Rust async |
| 栈在哪 | 独立 malloc 一块（几十 KB ~ 几 MB） | 编译器把协程状态拆成状态机，存在堆对象里 |
| 切换方式 | 汇编 swap 寄存器（100-500ns） | 函数调用 + 恢复状态机变量 |
| 能否跨栈抛异常 | 可以 | 不可以（必须在 frame 内处理） |
| 函数嵌套 Yield | 可以（任何深度都能 Yield） | 必须 `co_await` 点才能 suspend |
| 内存 | 整栈固定开销 | 按需分配，通常更小 |

**TinyRPC 是有栈**：看 `coctx_swap.S` 的汇编、看 `MainServerLoopCorFunc` 里嵌套调用 `input()→read_hook()→Yield()` 能在深层 frame 直接 Yield。

## 三、TinyRPC 协程核心概念

### 3.1 三种协程

| 协程 | 存在位置 | 跑什么 |
|---|---|---|
| **主协程** | 每个线程 thread_local，就是线程主栈本身 | `Reactor::loop()` |
| **m_accept_cor** | TcpServer 持有 | 主线程跑 `MainAcceptCorFunc`（死循环 accept） |
| **m_loop_cor** | 每个 TcpConnection 一个 | IO 线程跑 `MainServerLoopCorFunc`（input/execute/output） |

关键 thread_local 变量（`coroutine.cc:12-17`）：
```
t_main_coroutine    每线程主协程
t_cur_coroutine     当前正在跑的协程
t_cur_run_time      当前请求上下文（msg_no / interface_name 等）
```

### 3.2 Yield / Resume 的对称性

TinyRPC 的模型是**严格的主从模型**（非对称协程）：
- 只能 **主协程 Resume 子协程**
- 只能 **子协程 Yield 回主协程**
- 子协程不能直接 Resume 另一个子协程
- 所有协程都是"以主协程为枢纽"切换

代码证据（`coroutine.cc:172 / 187`）：
```cpp
void Coroutine::Yield() {
    if (t_cur_coroutine == t_main_coroutine) { ErrorLog; return; }  // 主协程不能 Yield
    coctx_swap(&(co->m_coctx), &(t_main_coroutine->m_coctx));
}
void Coroutine::Resume(Coroutine* co) {
    if (t_cur_coroutine != t_main_coroutine) { ErrorLog; return; } // 必须从主协程出发
    coctx_swap(&(t_main_coroutine->m_coctx), &(co->m_coctx));
}
```

### 3.3 协程启动：CoFunction 入口函数

一个子协程第一次被 Resume 时，RIP 跳到 `CoFunction`（不是直接跳到用户的 callback）：

```cpp
void CoFunction(Coroutine* co) {
    co->setIsInCoFunc(true);
    co->m_call_back();        // 真正跑用户逻辑
    co->setIsInCoFunc(false);
    Coroutine::Yield();       // 用户 callback 返回后自动 Yield 回主协程
}
```

这个包装层的作用：用户 callback 自然 return 时也能自动切回主协程，不需要用户手动 Yield。

### 3.4 栈怎么布置

`setCallBack` 时（`coroutine.cc:112-123`）做了这几件事：
1. `top = m_stack_sp + m_stack_size` 定位栈顶（x86 栈向下增长）
2. `top & -16LL` 做 16 字节对齐（x86-64 ABI 要求）
3. coctx 预设：
   - `kRSP = top`（栈指针）
   - `kRBP = top`（基址指针）
   - `kRETAddr = CoFunction`（第一次 swap 后 ret 就跳到这）
   - `kRDI = this`（x86-64 第一个参数寄存器，把 Coroutine* 传给 CoFunction）
4. `m_can_resume = true`（幂等保护，防止重复 Resume）

## 四、协程 vs 线程 vs 进程

| | 进程 | 线程 | 协程 |
|---|---|---|---|
| 调度者 | OS 内核 | OS 内核 | 用户态代码 |
| 切换代价 | ~10μs（换页表 + TLB flush） | ~1-5μs（内核态切换） | ~100-500ns（纯用户态） |
| 栈开销 | 独立地址空间 | 8MB 默认 | 几十~几百 KB |
| 通信 | IPC / 管道 / 共享内存 | 共享地址空间 + 锁 | 同线程共享变量，无锁 |
| 抢占 | 时间片 | 时间片 | 协作式（TinyRPC） / 异步抢占（Go 1.14+） |
| 并发数 | 几百~几千 | 几千~几万 | 几万~几十万 |

**切换为啥协程便宜这么多**：
- 不进内核（no syscall）→ 省掉陷入/返回、寄存器保存、调度器工作
- 不换页表 → TLB 命中不变
- 不经过调度器 → 没有优先级/CFS 红黑树计算
- 只换 ~14 个通用寄存器 + RSP + RIP

## 五、协作式调度的根本限制

**协程只在显式 Yield 时让出 CPU**。碰到以下情况**没法救**：

| 场景 | 协程能救吗 | 为什么 |
|---|---|---|
| `read` EAGAIN | ✅ | `read_hook` 内部 `toEpoll + Yield` |
| `accept` 无连接 | ✅ | `accept_hook` 同上 |
| `connect` 等 SYN-ACK | ✅ | `connect_hook` 同上 + 超时 timer |
| `sleep(5)` | ✅ | `sleep_hook` 挂 timer 再 Yield |
| **同步 MySQL query** | ❌ | syscall 没被 hook，线程卡在内核 |
| **CPU 密集 for 循环** | ❌ | 用户态不 Yield，自然没机会切 |
| **`std::mutex::lock()` 阻塞** | ❌ | futex syscall 没被 hook |
| **同步文件 IO (read/write 普通文件)** | ❌ | 磁盘 IO 在 Linux 上对 epoll 是 "always ready"，hook 的 epoll 模型救不了 |

**解法**：
1. 所有可能阻塞的调用都改成**异步版本或 hook 版本**
2. 真 CPU 密集 / 没法 hook 的阻塞 → 扔到独立业务线程池
3. 字节 brpc 的 bthread 支持 M:N，一个 worker 卡了其他 worker 能偷任务

## 六、高频八股 talking points

### Q: 协程切换比线程快几十倍，具体快在哪？
A: 不进内核 / 不换页表（TLB 不 flush）/ 不过调度器 / 只切 14 个寄存器 + RSP + RIP。10μs vs 300ns，差 30 倍。

### Q: 2000 个协程 vs 2000 个线程的内存开销？
A: 线程默认 8MB/栈 × 2000 = 16GB 虚拟内存；协程 128KB/栈 × 2000 = 256MB。相差 60+ 倍。实际 RSS 因 lazy allocation 小一些，但 VSZ 差距真实影响 fork/exec。

### Q: 有栈协程怎么保存寄存器？
A: x86-64 下 coctx 结构体存 14 个 callee-saved 寄存器 + RSP + RIP，`coctx_swap.S` 做 push/pop 汇编。caller-saved 由编译器在函数调用处已保存，不需要 coctx 管。

### Q: TinyRPC 的协程是抢占式的吗？
A: 不是，协作式。业务 handler 里死循环会卡死整个 IO 线程。Go 1.14 用 SIGURG 实现异步抢占，C++ 有栈协程做抢占极难（栈状态可能在任意指令，信号处理要极小心 + 不能打断某些 syscall）。

### Q: 协程为什么不能直接代替线程？
A: 协程没有真并行（同一线程内协程严格串行），要吃多核必须配线程池。所以生产级设计都是 **M 个 OS 线程 × N 个协程** 的 M:N 调度（Go / brpc）。

### Q: 有栈协程和 C++20 协程（co_await）区别？
A: 
- 有栈：汇编切寄存器+栈，任意嵌套深度可 Yield，栈固定开销
- 无栈：编译器改写成状态机，只能在 co_await 点 suspend，suspend 时的局部变量存在堆上的 coroutine frame 里，**不能**跨未 co_await 的普通函数 suspend
- 性能上无栈更省（不占整栈），但灵活性不如有栈

### Q: 怎么判断一个协程库是有栈还是无栈？
A: 看三点：① 有没有独立栈空间分配（malloc 栈）；② 能否在普通函数里直接 Yield 而不改函数签名；③ 有没有汇编 swap。有一即有栈。

