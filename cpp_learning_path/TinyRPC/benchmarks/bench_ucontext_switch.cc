// bench_ucontext_switch.cc
//
// 测量 ucontext 协程切换开销（glibc 标准库）
//
// 方法论:
//   两个协程互相 swapcontext() N 次
//   单次 swapcontext = 1 次协程切换
//
// 为什么 ucontext 比 coctx 慢:
//   swapcontext 内部会调用 sigprocmask 这个 syscall
//   (保存/恢复 signal mask)，每次切换多 ~200ns
//
// 编译: g++ -O2 bench_ucontext_switch.cc -o bench_ucontext_switch
//       (需要 _XOPEN_SOURCE 才能用 ucontext，已 deprecated 但仍可用)

#define _XOPEN_SOURCE 700
#include <ucontext.h>
#include <cstdio>
#include <chrono>

constexpr int N = 10'000'000; // 协程切换足够快，多测几次

ucontext_t ctx_main, ctx_a, ctx_b;
int counter = 0;

void func_a() {
    while (counter < N) {
        ++counter;
        swapcontext(&ctx_a, &ctx_b);
    }
    // 最后跳回 main
    swapcontext(&ctx_a, &ctx_main);
}

void func_b() {
    while (counter < N) {
        ++counter;
        swapcontext(&ctx_b, &ctx_a);
    }
    swapcontext(&ctx_b, &ctx_main);
}

int main() {
    constexpr int STACK_SZ = 64 * 1024;
    static char stack_a[STACK_SZ];
    static char stack_b[STACK_SZ];

    getcontext(&ctx_a);
    ctx_a.uc_stack.ss_sp = stack_a;
    ctx_a.uc_stack.ss_size = STACK_SZ;
    ctx_a.uc_link = &ctx_main;
    makecontext(&ctx_a, func_a, 0);

    getcontext(&ctx_b);
    ctx_b.uc_stack.ss_sp = stack_b;
    ctx_b.uc_stack.ss_size = STACK_SZ;
    ctx_b.uc_link = &ctx_main;
    makecontext(&ctx_b, func_b, 0);

    auto t0 = std::chrono::steady_clock::now();

    swapcontext(&ctx_main, &ctx_a); // 启动

    auto t1 = std::chrono::steady_clock::now();
    double elapsed_ns = std::chrono::duration<double, std::nano>(t1 - t0).count();

    double per_switch_ns = elapsed_ns / counter;

    printf("==== ucontext Coroutine Switch Benchmark ====\n");
    printf("Switches         : %d\n", counter);
    printf("Total time       : %.2f ms\n", elapsed_ns / 1e6);
    printf("Per-switch time  : %.1f ns\n", per_switch_ns);
    printf("\n");
    printf("注: ucontext 慢的原因 = sigprocmask syscall\n");
    printf("   (保存/恢复 signal mask, 每次切换多 ~200ns)\n");
    return 0;
}
