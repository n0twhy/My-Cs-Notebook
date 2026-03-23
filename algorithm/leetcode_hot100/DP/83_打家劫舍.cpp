/*
 * LeetCode 198 - 打家劫舍 (House Robber)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的
 * 唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上
 * 被小偷闯入，系统会自动报警。
 *
 * 给定一个代表每个房屋存放金额的非负整数数组，计算你不触动警报装置的情况下，
 * 一夜之内能够偷窃到的最高金额。
 *
 * 【输入格式】
 * 一个代表每个房屋存放金额的非负整数数组（空格分隔）。
 *
 * 【输出格式】
 * 一行一个整数，表示一夜之内能够偷窃到的最高金额。
 *
 * 【样例输入 1】
 * 1 2 3 1
 *
 * 【样例输出 1】
 * 4
 *
 * （偷窃 1 号房屋和 3 号房屋，最高金额为 1 + 3 = 4）
 *
 * 【样例输入 2】
 * 2 7 9 3 1
 *
 * 【样例输出 2】
 * 12
 *
 * （偷窃 1、3、5 号房屋，最高金额为 2 + 9 + 1 = 12）
 *
 * 【数据范围】
 * 1 <= nums.length <= 100
 * 0 <= nums[i] <= 400
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int MaxStole(vector<int>& nums) {
    vector<int> dp(nums.size());
    dp[0] = nums[0];
    dp[1] = nums[1];

    for (int i = 2; i < nums.size(); ++i) {
      dp[i] = max(dp[i - 2] + nums[i], dp[i - 1]);
    }

    return max(dp[nums.size() - 1], dp[nums.size() - 2]);
  }
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  vector<int> nums;
  int n;
  while (ss >> n) {
    nums.emplace_back(n);
  }
  Solution sol;
  cout << sol.MaxStole(nums) << endl;
}