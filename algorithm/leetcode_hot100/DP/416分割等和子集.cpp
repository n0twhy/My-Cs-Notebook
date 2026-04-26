/*
 * LeetCode 416 - 分割等和子集 (Partition Equal Subset Sum)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个只包含正整数的非空数组 nums，判断是否可以将数组分割成两个子集，使得两个子集的元素和相等。
 *
 * 【输入格式】
 * 第一行：一个整数 n，表示数组长度。
 * 第二行：n 个正整数，空格分隔，表示数组 nums。
 *
 * 【输出格式】
 * 一行：若可以分割成两个和相等的子集输出 "true"，否则输出 "false"（无引号、小写）。
 *
 * 【样例输入 1】
 * 4
 * 1 5 11 5
 *
 * 【样例输出 1】
 * true
 *
 * （可分割为 [1,5,5] 与 [11]，和均为 11）
 *
 * 【样例输入 2】
 * 4
 * 1 2 3 5
 *
 * 【样例输出 2】
 * false
 *
 * 【数据范围】
 * 1 <= n <= 200
 * 1 <= nums[i] <= 100
 *
 * =============================================================================
 */
#include <bits/stdc++.h>

using namespace std;
class Solution {
public:
  bool split(vector<int>& nums) {
    int sum = 0;
    for (auto& i : nums) {
      sum += i;
    }

    if(sum % 2) {
      return false;
    }

    sum /= 2;
    
    vector<bool> dp(sum + 1, false);
    dp[0] = true;
    for (int& num : nums) {
      for (int j = sum; j >= num; --j) {
        dp[j] = dp[j - num] || dp[j];
      }
    }
    return dp[sum];
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
  bool res = sol.split(nums);
  if (res) {
    cout << "true" << endl;
  } else {
    cout << "false" << endl;
  }
}