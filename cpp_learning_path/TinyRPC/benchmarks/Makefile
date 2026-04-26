COCTX_S = /home/millionm1nd/My-Cs-Notebook/project/tinyrpc/tinyrpc/coroutine/coctx_swap.S

CXXFLAGS = -O2 -std=c++17 -Wall

all: bench_thread_switch bench_ucontext_switch bench_coctx_switch

bench_thread_switch: bench_thread_switch.cc
	g++ $(CXXFLAGS) -pthread $< -o $@

bench_ucontext_switch: bench_ucontext_switch.cc
	g++ $(CXXFLAGS) $< -o $@

bench_coctx_switch: bench_coctx_switch.cc
	g++ $(CXXFLAGS) $< $(COCTX_S) -o $@

run: all
	@echo "========================================"
	@echo "  Context Switch Benchmark Suite"
	@echo "========================================"
	@echo ""
	@./bench_thread_switch
	@echo ""
	@./bench_ucontext_switch
	@echo ""
	@./bench_coctx_switch
	@echo ""
	@echo "========================================"
	@echo "  Summary"
	@echo "========================================"

clean:
	rm -f bench_thread_switch bench_ucontext_switch bench_coctx_switch
