// bench_thread_switch.cc
//
// 测量 Linux 线程上下文切换开销（同核 ping-pong）
//
// 方法论:
//   创建两个线程 A/B，通过两根 pipe 互发 1 字节做 ping-pong:
//     A write -> B read -> B write -> A read  (= 2 次线程切换)
//   重复 N 次，总时间 / (2N) = 单次线程切换耗时
//
// 关键细节:
//   1. 两线程绑到同一个 CPU 核，避免测的是"跨核通信"
//   2. read/write 每次都会触发 futex/wakeup，是真实的内核级切换
//   3. 包含了 syscall 本身的开销 (~100ns) + 调度器开销
//
// 编译: g++ -O2 -pthread bench_thread_switch.cc -o bench_thread_switch

#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>

constexpr int N = 1'000'000;

int pipe_a2b[2]; // A 写 B 读
int pipe_b2a[2]; // B 写 A 读

void pin_to_cpu(int cpu) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
}

void* thread_a(void*) {
    pin_to_cpu(2); // 绑 CPU 2
    char buf;
    for (int i = 0; i < N; ++i) {
        write(pipe_a2b[1], "x", 1);
        read(pipe_b2a[0], &buf, 1);
    }
    return nullptr;
}

void* thread_b(void*) {
    pin_to_cpu(2); // 同一个核
    char buf;
    for (int i = 0; i < N; ++i) {
        read(pipe_a2b[0], &buf, 1);
        write(pipe_b2a[1], "x", 1);
    }
    return nullptr;
}

int main() {
    if (pipe(pipe_a2b) || pipe(pipe_b2a)) {
        perror("pipe"); return 1;
    }

    // 预热: 跑 10000 次丢弃，让 cache / TLB 稳定
    // (略, N 足够大)

    auto t0 = std::chrono::steady_clock::now();

    pthread_t a, b;
    pthread_create(&a, nullptr, thread_a, nullptr);
    pthread_create(&b, nullptr, thread_b, nullptr);
    pthread_join(a, nullptr);
    pthread_join(b, nullptr);

    auto t1 = std::chrono::steady_clock::now();
    double elapsed_ns = std::chrono::duration<double, std::nano>(t1 - t0).count();

    // 每次循环 2 次切换
    double per_switch_ns = elapsed_ns / (2.0 * N);

    printf("==== Thread Context Switch Benchmark ====\n");
    printf("Iterations       : %d rounds (= %d switches)\n", N, 2 * N);
    printf("Total time       : %.2f ms\n", elapsed_ns / 1e6);
    printf("Per-switch time  : %.1f ns\n", per_switch_ns);
    printf("Per-switch time  : %.3f us\n", per_switch_ns / 1000.0);
    printf("\n");
    printf("注: 此数字包含\n");
    printf("  - 2 次 syscall (read + write) ~200ns\n");
    printf("  - futex 唤醒 + 调度器 pick_next\n");
    printf("  - TLB/cache miss (同核 ping-pong 下最小化)\n");
    return 0;
}
