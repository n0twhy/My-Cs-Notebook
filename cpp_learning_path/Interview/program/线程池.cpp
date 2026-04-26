#include <bits/stdc++.h>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <random>
#include <set>
#include <thread>
using namespace std;

class ThreadPool {
public:
  ThreadPool(int n) : tp_cnt_(n) {
    for (size_t i = 0; i < tp_cnt_; ++i) {
      threads_.emplace_back([&](){
        while (true) {
          unique_lock<mutex> lock(latch_);
          if (stop_) break;
          cv_.wait(lock, [&](){
            return !tasks_.empty() || stop_;
          });
          auto task = tasks_.front();
          tasks_.pop();
          lock.unlock();
          task();
        }
      });
    }
  }
  ~ThreadPool() {
    SetStop();
    for (auto& it : threads_) {
      it.join();
    }
  }

  void submit(function<void()> task) {
    if (stop_) return;

    unique_lock<mutex> lock(latch_);    
    tasks_.push(task);
    lock.unlock();
    cv_.notify_one();
  }

  void SetStop() {
    stop_ = true;
    cv_.notify_all();
  }

private:
  int tp_cnt_;
  vector<thread> threads_;
  queue<function<void()>> tasks_;
  mutex latch_;
  condition_variable cv_;
  bool stop_ {false};
};

int main() {
  int threads_cnt = 5;
  auto tp = ThreadPool(threads_cnt);
  atomic<int> sum(0);
  
  function<void()> mission = [&sum](){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 99);
    this_thread::sleep_for(chrono::milliseconds(dis(gen)));
    sum++;
    cout << "now sum = " << sum << endl;
  };

  for (int i = 0; i < 20; ++i) {
    tp.submit(mission);
  }

  cout << sum << endl;

  return 0;
}