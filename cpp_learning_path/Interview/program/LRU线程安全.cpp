#include <bits/stdc++.h>
#include <list>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
using namespace std;

template<typename K, typename V>
class LRU {
public:
  optional<V> get(const K& key) {
    unique_lock<shared_mutex> lock(mtx_);
    auto it = mp_.find(key);
    if (it == mp_.end()) return nullopt;
    lst_.splice(lst_.begin(), lst_, it->second);
    return it->second->second;
  }

  void put(const K &key, const V &value) {
    unique_lock<shared_mutex> lock(mtx_);
    auto it = mp_.find(key);
    if (it != mp_.end()) {
      it->second->second = move(value);
      lst_.splice(lst_.begin(), lst_, it->second);
      return;
    }
    lst_.emplace_front({key, value});
    if (lst_.size() > capacity_) {
      mp_.erase(lst_.back().first);
      lst_.pop_back();
    }

    mp_[key] = lst_.begin();
  }

private:
  int capacity_;
  list<pair<K, V>> lst_;
  unordered_map<K, typename list<pair<K, V>>::iterator> mp_;
  mutable shared_mutex mtx_;
};