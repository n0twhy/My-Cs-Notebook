/*
题目：数组取 min

题目内容
小明拿到了一个长度为 n 的数组，他有一种操作：
将数组里的每个数都替换为 min(a_i, x)，其中 x 由小明自行选定。

小明想知道：在数组总和不超过 tot 的情况下，x 最大可以是多少？

如果数组总和原本就已经小于等于 tot，则直接输出 -1。

输入描述
第一行输入两个整数 n、tot（1 <= n, tot <= 100000）。
第二行输入 n 个整数，表示数组元素（1 <= a_i <= 100000）。

输出描述
输出一个整数，表示 x 最大可以取到多少；
如果不需要进行操作则输出 -1。

样例一
输入
5 20
3 2 5 12 6

输出
5

样例二
输入
5 20
1 1 1 1 1

输出
-1
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <functional>
#include <vector>
using namespace std;

class Solution {
public:
  int ArrayMin(vector<int> &nums, int tot) {
    sort(nums.begin(), nums.end());
    
    vector<int> pre_sum;
    int sum = 0;
    for (auto i : nums) {
      sum += i;
      pre_sum.emplace_back(sum);
    }

    if (sum < tot) return -1;

    int x = -1;
    int left = 0;
    int right = nums[nums.size() - 1];

    while (left <= right) {
      int mid = (left + right) / 2;
      auto it = upper_bound(nums.begin(), nums.end(), mid);
      int pos = it - nums.begin();
      int curr_sum = 0;
      if (pos == 0) {
        curr_sum = nums.size() * mid;
      } else {
        curr_sum = pre_sum[pos - 1] + (nums.size() - pos) * mid;
      }
      
      if (curr_sum > tot) {
        right = mid - 1;
      } 
      if (curr_sum <= tot) {
        x = mid;
        left = mid + 1;
      }
    }

    return x;
  }
};

int main() {
  int n, tot;
  cin >> n >> tot;
  vector<int> nums(n);
  for (int i = 0; i < n; ++i) {
    cin >> nums[i];
  }

  Solution sol;
  cout << sol.ArrayMin(nums, tot) << endl;
}