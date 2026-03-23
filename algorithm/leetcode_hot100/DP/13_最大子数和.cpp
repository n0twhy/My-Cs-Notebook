/*
 * LeetCode 53 - 最大子数组和 (Maximum Subarray)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给你一个整数数组 nums，请你找出一个具有最大和的连续子数组
 * （子数组最少包含一个元素），输出其最大和。
 *
 * 子数组是数组中的一个连续部分。
 *
 * 【输入格式】
 * 输入共两行。
 * 第一行为一个整数 n，代表数组 nums 的长度。
 * 第二行为 n 个整数 nums[0], nums[1], ..., nums[n-1]，数字之间以空格分隔。
 *
 * 【输出格式】
 * 一行一个整数，表示答案。
 *
 * 【样例输入 1】
 * 9
 * -2 1 -3 4 -1 2 1 -5 4
 *
 * 【样例输出 1】
 * 6
 *
 * （连续子数组 [4, -1, 2, 1] 的和最大，为 6）
 *
 * 【样例输入 2】
 * 1
 * 1
 *
 * 【样例输出 2】
 * 1
 *
 * 【样例输入 3】
 * 5
 * 5 4 -1 7 8
 *
 * 【样例输出 3】
 * 23
 *
 * 【数据范围】
 * 1 <= nums.length <= 10^5
 * -10^4 <= nums[i] <= 10^4
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int SumSubarray(vector<int> &nums) {
    vector<int> dp(nums.size());
    dp[0] = nums[0];
    int res = dp[0];
    
    for (int i = 1; i < nums.size(); ++i) {
      dp[i] = max(nums[i] + dp[i - 1], nums[i]);
      res = max(res, dp[i]);
    }

    return res;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> nums(n);
  for (int i = 0; i < n; ++i) {
    cin >> nums[i];
  }

  Solution sol;
  cout << sol.SumSubarray(nums) << endl;
}
