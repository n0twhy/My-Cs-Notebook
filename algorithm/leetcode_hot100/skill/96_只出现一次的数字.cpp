/*
 * LeetCode 136 - 只出现一次的数字 (Single Number)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给你一个非空整数数组 nums，除了某个元素只出现一次以外，
 * 其余每个元素均出现两次。找出那个只出现了一次的元素。
 *
 * 你必须设计并实现线性时间复杂度的算法来解决此问题，且该算法只使用常量额外空间。
 *
 * 【输入格式】
 * 一行：一个整数数组 nums（空格分隔）。
 *
 * 【输出格式】
 * 一行一个整数，表示只出现了一次的元素。
 *
 * 【样例输入 1】
 * 2 2 1
 *
 * 【样例输出 1】
 * 1
 *
 * 【样例输入 2】
 * 4 1 2 1 2
 *
 * 【样例输出 2】
 * 4
 *
 * 【样例输入 3】
 * 1
 *
 * 【样例输出 3】
 * 1
 *
 * 【数据范围】
 * 1 <= nums.length <= 3 * 10^4
 * -3 * 10^4 <= nums[i] <= 3 * 10^4
 * 除了某个元素只出现一次以外，其余每个元素均出现两次
 *
 * =============================================================================
 */

 #include <bits/stdc++.h>
 using namespace std;

class Solution {
public:
  int single(vector<int>& nums) {
    int ans = 0;
    for (auto& i : nums) {
      ans ^= i;
    }

    return ans;
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
  cout << sol.single(nums);
}