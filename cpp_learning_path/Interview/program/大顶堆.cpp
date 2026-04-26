#include <bits/stdc++.h>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  void Heapify(vector<int> &nums, int pos) {
    int l = pos * 2 + 1;
    int r = pos * 2 + 2;
    int bigger = pos;
    if (l < nums.size() && nums[bigger] < nums[l]) {
      bigger = l;
    }
    if (r < nums.size() && nums[bigger] < nums[r]) {
      bigger = r;
    }
    if (bigger == pos) return;
    swap(nums[bigger], nums[pos]);
    if (bigger == l) Heapify(nums, l);
    if (bigger == r) Heapify(nums, r);
  }

  void BuildHeap(vector<int> &nums) {
    int n = nums.size();
    int last_parent_node_pos = (n - 1) / 2;

    for (int i = last_parent_node_pos; i >= 0; i--) {
      Heapify(nums, i);
    }
  }
};

int main() {
  vector<int> nums = {1, 2, 3, 6, 10, 99};
  Solution sol;
  sol.BuildHeap(nums);
  for (int i = 0; i < nums.size(); ++i) {
    cout << nums[i] << endl;
  }
}