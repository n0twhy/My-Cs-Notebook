#include <bits/stdc++.h>
using namespace std;

template <typename T>
class BlockingQueue{
public:
  void push(const T& value) {
    unique_lock<mutex> lock(latch_);
    if (closed) {
      throw runtime_error("queue has been closed.");
    }
    q_.push(value);
    cv_.notify_all();
  }
  bool pop(T& out) {
    unique_lock<mutex> lock(latch_);
    cv_.wait(lock, [&](){
      return closed || !q_.empty();
    });
    
    if (q_.empty()) {
      return false;
    }

    out = q_.front();
    q_.pop();
    return true;
  }
  bool try_pop(T& out) {
    unique_lock<mutex> lock(latch_);
    if (q_.empty()) {
      return false;
    }

    out = move(q_.front());
    q_.pop();
    return true;
  }
  void close() {
    unique_lock<mutex> lock(latch_);
    closed = true;
    cv_.notify_all();    
  }
private:
  queue<T> q_;
  mutex latch_;
  condition_variable cv_;
  bool closed {false};
};

int main() {
  BlockingQueue<int> q;
  atomic<int> sum(0);

  const int producer_cnt = 3;
  const int consumer_cnt = 2;
  const int items_per_producer = 10;

  vector<thread> producers;
  vector<thread> consumers;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 99);

  for (int i = 0; i < consumer_cnt; ++i) {
    consumers.emplace_back([&, i](){
      int x;
      while (q.pop(x)) {
        sum++;
        this_thread::sleep_for(chrono::milliseconds(dis(gen)));
        cout << "consumer[" << i << "] got " << x << endl;
      }
      cout << "consumer[" << i << "] exit" << endl;
    });
  }

  for (int i = 0; i < producer_cnt; ++i) {
    producers.emplace_back([&, i]() {
      for (int j = 0; j < items_per_producer; ++j) {
        int value = dis(gen);
        this_thread::sleep_for(chrono::milliseconds(dis(gen)));
        q.push(value);
      }
      cout << "producer[" << i << "] exits" << endl;
    });
  }

  for (auto &t : producers) {
    t.join();
  }

  q.close();

  for (auto& t: consumers) {
    t.join();
  }

  cout << "done, sum = " << sum << endl;
  return 0;
}

