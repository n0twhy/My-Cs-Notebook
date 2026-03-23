/*
 * LeetCode 279 - 完全平方数 (Perfect Squares)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定正整数 n，返回和为 n 的完全平方数的最少个数。完全平方数指 1、4、9、16、25 … 等。
 *
 * 【输入格式】
 * 一行，一个正整数 n。
 *
 * 【输出格式】
 * 一行一个整数，表示最少需要的完全平方数个数。
 *
 * 【样例输入 1】
 * 12
 *
 * 【样例输出 1】
 * 3
 *
 * （12 = 4 + 4 + 4）
 *
 * 【样例输入 2】
 * 13
 *
 * 【样例输出 2】
 * 2
 *
 * （13 = 4 + 9）
 *
 * 【数据范围】
 * 1 <= n <= 10^4
 *
 * =============================================================================
 */
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
  int mininum(int n) {
    vector<int> dp(n + 1, n);
    dp[0] = 0;
    for (int i = 1; i <= n; ++i) {
      for (int j = 1; j * j <= i; ++j) {
        dp[i] = min(dp[i], dp[i - j * j] + 1);
      }
    }
    return dp[n];
    
  }
};

int main() {
  int n;
  cin >> n;
  Solution sol;
  cout << sol.mininum(n) << endl;
}