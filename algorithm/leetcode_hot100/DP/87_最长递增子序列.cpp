/*
 * LeetCode 300 - 最长递增子序列 (Longest Increasing Subsequence)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给你一个整数数组 nums，找到其中最长严格递增子序列的长度。
 *
 * 子序列是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。
 * 例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。
 *
 * 【输入格式】
 * 一行：一个整数数组 nums（空格分隔）。
 *
 * 【输出格式】
 * 一行一个整数，表示最长严格递增子序列的长度。
 *
 * 【样例输入 1】
 * 10 9 2 5 3 7 101 18
 *
 * 【样例输出 1】
 * 4
 *
 * （最长递增子序列是 [2,3,7,101]，因此长度为 4）
 *
 * 【样例输入 2】
 * 0 1 0 3 2 3
 *
 * 【样例输出 2】
 * 4
 *
 * 【样例输入 3】
 * 7 7 7 7 7 7 7
 *
 * 【样例输出 3】
 * 1
 *
 * 【数据范围】
 * 1 <= nums.length <= 2500
 * -10^4 <= nums[i] <= 10^4
 *
 * =============================================================================
 */
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int maxSeqSubarray(vector<int> &nums) {
    vector<int> dp(nums.size(), 1);
    int res = 0;
    for (int i = 0; i < nums.size(); ++i) {
      int curr = 1;
      for (int j = 0; j < i; ++j) {
        if (nums[i] <= nums[j]) continue;
        curr = max(curr, dp[j] + 1); 
      }
      dp[i] = curr;
      res = max(res, dp[i]);
    }
    
    return res;
  }
};

int main() {
  string tmp;
  getline(cin, tmp);
  stringstream ss(tmp);
  int n;
  vector<int> nums;
  while (ss >> n) {
    nums.emplace_back(n);
  }
  Solution sol;
  cout << sol.maxSeqSubarray(nums) << endl;
}