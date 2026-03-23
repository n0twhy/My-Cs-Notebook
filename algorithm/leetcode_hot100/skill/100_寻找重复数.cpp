/*
 * LeetCode 287 - 寻找重复数 (Find the Duplicate Number)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个包含 n + 1 个整数的数组 nums，其数字都在 [1, n] 范围内
 * （包括 1 和 n），可知至少存在一个重复的整数。
 *
 * 假设 nums 只有一个重复的整数，返回这个重复的数。
 *
 * 你的解决方案必须不修改数组 nums，且只用常量级 O(1) 的额外空间。
 *
 * 【输入格式】
 * 输入包含两行：
 * 第一行输入一个整数 n（1 <= n <= 10^5），表示数组中数字范围上界为 n，
 * 且 nums 的长度为 n + 1。
 * 第二行输入 n + 1 个整数，表示数组 nums。
 *
 * 【输出格式】
 * 一行一个整数，表示 nums 中唯一的重复整数。
 *
 * 【样例输入 1】
 * 4
 * 1 3 4 2 2
 *
 * 【样例输出 1】
 * 2
 *
 * 【数据范围】
 * nums 的长度始终为 n + 1
 * nums 中所有数字都在 [1, n] 范围内
 * nums 中只有一个整数出现两次或多次，其余整数均只出现一次
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int duplication(vector<int> &nums) {
    int slow = 0;
    int fast = 0;

    while (nums[slow] != nums[fast]) {
      slow = nums[nums[slow]];
      fast = nums[nums[nums[fast]]];
    }

    return nums[fast];
  } 
};

int main() {
  int n;
  cin >> n;
  vector<int> nums(n + 1);
  for (int i = 0; i <= n; ++i) {
    cin >> nums[i];
  }
  Solution sol;
  cout << sol.duplication(nums) << endl;
}