/*
 * LeetCode 152 - 乘积最大子数组 (Maximum Product Subarray)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给你一个整数数组 nums，请你找出数组中乘积最大的非空连续子数组
 * （该子数组中至少包含一个数字），并返回该子数组所对应的乘积。
 *
 * 测试用例的答案是一个 32 位整数。
 *
 * 【输入格式】
 * 一行：一个整数数组 nums（空格分隔）。
 *
 * 【输出格式】
 * 一行一个整数，表示最大乘积。
 *
 * 【样例输入 1】
 * 2 3 -2 4
 *
 * 【样例输出 1】
 * 6
 *
 * （子数组 [2,3] 有最大乘积 6）
 *
 * 【样例输入 2】
 * -2 0 -1
 *
 * 【样例输出 2】
 * 0
 *
 * （结果不能为 2，因为 [-2,-1] 不是子数组）
 *
 * 【数据范围】
 * 1 <= nums.length <= 2 * 10^4
 * -10 <= nums[i] <= 10
 * nums 的任何子数组的乘积都保证是一个 32 位整数
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int MaxMultiple(vector<int> &nums) {
    vector<int> dp_min(nums.size());
    vector<int> dp_max(nums.size());
    dp_min[0] = dp_max[0] = nums[0];
    int res = nums[0];

    for (int i = 1; i < nums.size(); ++i) {
      dp_max[i] = max({nums[i], nums[i] * dp_max[i - 1], nums[i] * dp_min[i - 1]});
      dp_min[i] = min({nums[i], nums[i] * dp_max[i - 1], nums[i] * dp_min[i - 1]});
      res = max(res, dp_max[i]);
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
  cout << sol.MaxMultiple(nums) << endl;
}
