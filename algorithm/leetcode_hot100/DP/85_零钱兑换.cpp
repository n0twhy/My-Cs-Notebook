/*
 * LeetCode 322 - 零钱兑换 (Coin Change)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给你一个整数数组 coins，表示不同面额的硬币；以及一个整数 amount，表示总金额。
 *
 * 计算并返回可以凑成总金额所需的最少硬币个数。如果没有任何一种硬币组合能组成总金额，
 * 返回 -1。
 *
 * 你可以认为每种硬币的数量是无限的。
 *
 * 【输入格式】
 * 第一行：一个整数数组 coins（空格分隔）。
 * 第二行：一个整数 amount。
 *
 * 【输出格式】
 * 一行一个整数，表示可以凑成总金额所需的最少硬币个数。
 *
 * 【样例输入 1】
 * 1 2 5
 * 11
 *
 * 【样例输出 1】
 * 3
 *
 * （11 = 5 + 5 + 1）
 *
 * 【样例输入 2】
 * 2
 * 3
 *
 * 【样例输出 2】
 * -1
 *
 * 【样例输入 3】
 * 1
 * 0
 *
 * 【样例输出 3】
 * 0
 *
 * 【数据范围】
 * 1 <= coins.length <= 12
 * 1 <= coins[i] <= 2^31 - 1
 * 0 <= amount <= 10^4
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
#include <climits>
#include <cstdint>
using namespace std;

class Solution {
public:
  int MoneyConversion(vector<int>& coins, int key) {
    vector<int> dp(key + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i <= key; ++i) {
      for (int j = 0; j < coins.size(); ++j) {
        if (coins[j] > i || dp[i - coins[j]] == INT_MAX) continue;
        dp[i] = min(dp[i], dp[i - coins[j]] + 1);
      }
    }
    return dp[key] == INT_MAX ? 0 : dp[key];
  }
};

int main() {
  string tmp;
  getline(cin, tmp);
  stringstream ss(tmp);
  int n;
  vector<int> coins;
  while (ss >> n) {
    coins.emplace_back(n);
  }
  int key;
  cin >> key;
  Solution sol;
  cout << sol.MoneyConversion(coins, key);
}
