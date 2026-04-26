// ucontext_mini.cc
// 编译: g++ -O0 -g ucontext_mini.cc -o ucontext_mini
// 目标: 一个主协程 <-> 一个子协程来回切换 3 次，打印交替执行

#include <string>
#include <ucontext.h>
#include <cstdio>
#include <cstdlib>

static ucontext_t g_main_ctx;   // 主协程上下文
static ucontext_t g_co_ctx;     // 子协程上下文

// ========== TODO 1: 栈分配 ==========
// 子协程需要一块独立的栈。思考:
//   - 栈多大? (tinyrpc 默认 128KB, 这里给 64KB 够不够?)
//   - 用 new / malloc / mmap? 差别是什么?
//   - 栈是从低地址向高地址增长还是反过来? ss_sp 应该指哪?
static constexpr size_t kStackSize = 128 * 1024; // <-- 改成合适的值
static char* g_co_stack = nullptr;      // <-- 在 main 里分配

// 子协程入口函数
void co_entry(int arg1, int arg2) {
    // 把 arg1/arg2 还原为一个指针（makecontext 只能传 int，所以要拆/拼）
    // 这里暂时不用,先印出来验证
    printf("[co]   entered, args=(%d,%d)\n", arg1, arg2);
    
    for (int i = 0; i < 3; ++i) {
        printf("[co]   round %d, yield back to main\n", i);
        // ========== TODO 2: yield 回主协程 ==========
        // 提示: swapcontext(&?, &?)
        //   - 第一个参数: 把谁的上下文保存下来?
        //   - 第二个参数: 切换到谁?
        swapcontext(&g_co_ctx, &g_main_ctx);
        printf("[co]   back %d\n", i);
    }
    swapcontext(&g_main_ctx, &g_co_ctx);
    printf("[co]   finished, will implicitly return to uc_link\n");
    // 注意: 这里函数 return 之后会自动切到 uc_link 指向的上下文
}

int main() {
    // ========== TODO 1 (续): 分配栈 ==========
    // g_co_stack = ???
    // 步骤 A: 拿一个合法的上下文打底
    getcontext(&g_co_ctx);
    g_co_ctx.uc_stack.ss_sp = g_co_stack;
    g_co_stack = new char[kStackSize];
    // 步骤 B: 给这个上下文装上"自己的栈"
    g_co_ctx.uc_stack.ss_sp   = g_co_stack;   // 栈基址
    g_co_ctx.uc_stack.ss_size = kStackSize;   // 栈大小
    g_co_ctx.uc_link          = &g_main_ctx;  // 子协程 return 后去哪

    // ========== TODO 3: makecontext 参数 ==========
    // makecontext(&g_co_ctx, (void(*)())co_entry, ?, ?, ?);
    //   - 第 3 个参数 argc 填几?
    //   - co_entry 接收的是 int, 但我想传一个 std::string* 进去怎么办?
    //     (先不做,思考一下)
    makecontext(&g_co_ctx, (void(*)())co_entry, 2, 1, 2);
    printf("[main] ready, switch to coroutine\n");
    for (int i = 0; i < 3; ++i) {
      printf("[main] coroutine Resume#%d\n", i);
      swapcontext(&g_main_ctx, &g_co_ctx);
      printf("[main] back from yield #%d\n", i);
    }
    // ========== TODO 4: 首次启动子协程 ==========
    // 提示: 我既要保存主协程,又要跳进子协程 -> 用哪个函数?
    //   swapcontext(&?, &?);
    // 这行是子协程结束后回来打印的
    printf("[main] coroutine finished, back in main\n");

    free(g_co_stack);
    return 0;
}