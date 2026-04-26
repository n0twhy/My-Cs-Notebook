// bench_coctx_switch.cc
//
// 测量 TinyRPC coctx_swap (纯汇编) 协程切换开销
//
// 方法论:
//   直接调用 coctx_swap 在两个 coctx 之间 ping-pong
//   不涉及业务逻辑、不涉及 Coroutine 类的额外开销
//   测的就是"最纯粹的汇编级寄存器切换"
//
// coctx_swap 做了什么 (coctx_swap.S):
//   - 保存 14 个通用寄存器 + RSP + RIP 到当前 coctx
//   - 从目标 coctx 恢复这 16 个值
//   - ret (跳转到新 RIP)
//   = 约 30 条 mov + 1 ret，无 syscall、无内核态切换
//
// 编译:
//   g++ -O2 bench_coctx_switch.cc <项目路径>/coctx_swap.S -o bench_coctx_switch

#include <cstdio>
#include <chrono>
#include <cstring>

// 对齐 TinyRPC 的定义
struct coctx {
    void* regs[14];
};

extern "C" void coctx_swap(coctx*, coctx*) asm("coctx_swap");

constexpr int N = 50'000'000;
constexpr int STACK_SZ = 64 * 1024;

static coctx ctx_main;
static coctx ctx_worker;
static char worker_stack[STACK_SZ];
static int counter = 0;

// 协程函数: 不断 swap 回 main
static void worker_func() {
    while (true) {
        ++counter;
        coctx_swap(&ctx_worker, &ctx_main);
    }
}

// 按 TinyRPC coroutine.cc 的方式初始化 coctx
// 参考 kRBP=6, kRETAddr=9, kRSP=13
static void make_coctx(coctx* ctx, char* stack, int size, void(*entry)()) {
    memset(ctx, 0, sizeof(coctx));
    char* sp = stack + size;
    // 16 字节对齐 (SysV ABI 要求)
    sp = (char*)((uintptr_t)sp & ~0xF);
    sp -= 8; // 为 ret 预留一个"假返回地址"位置

    ctx->regs[13] = sp;               // RSP
    ctx->regs[9]  = (void*)entry;     // RIP (ret 跳转目标)
    ctx->regs[6]  = sp;               // RBP
}

int main() {
    make_coctx(&ctx_worker, worker_stack, STACK_SZ, worker_func);

    auto t0 = std::chrono::steady_clock::now();

    // 每次循环 = 2 次 coctx_swap (main->worker, worker->main)
    for (int i = 0; i < N; ++i) {
        coctx_swap(&ctx_main, &ctx_worker);
    }

    auto t1 = std::chrono::steady_clock::now();
    double elapsed_ns = std::chrono::duration<double, std::nano>(t1 - t0).count();

    int total_switches = 2 * N;
    double per_switch_ns = elapsed_ns / total_switches;

    printf("==== coctx (TinyRPC assembly) Coroutine Switch Benchmark ====\n");
    printf("Switches         : %d\n", total_switches);
    printf("Total time       : %.2f ms\n", elapsed_ns / 1e6);
    printf("Per-switch time  : %.2f ns\n", per_switch_ns);
    printf("\n");
    printf("注: coctx_swap 纯汇编\n");
    printf("   - 只存 14 通用寄存器 + RSP + RIP\n");
    printf("   - 无 syscall、无 signal mask 操作\n");
    printf("   - 理论下限 ~20ns (现代 CPU 30 条 mov + ret)\n");
    return 0;
}
