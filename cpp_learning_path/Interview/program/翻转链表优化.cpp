#include <bits/stdc++.h>
#include <memory>
#include <utility>
using namespace std;


template<typename T>
struct ListNode {
  T val;
  unique_ptr<ListNode<T>> next {nullptr};

  ListNode() : val(0) {}
  template<typename U>
  ListNode(U &&v) : val(forward<U>(v)) {}
};

template<typename T>
class Solution {
public:
  ListNode<T> *ReverseList(ListNode<T> *head) {
    ListNode<T> *prev = nullptr;
    ListNode<T> *curr = head;

    while (curr) {
      auto next = move(curr->next);
      curr->next.reset(prev);
      prev = curr;
      curr = next.get();
    }

    return prev;
  }
};
