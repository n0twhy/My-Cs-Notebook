/*
 * LeetCode 62 - 不同路径 (Unique Paths)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 一个机器人位于一个 m x n 网格的左上角（起始点记为 Start）。
 * 机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角
 * （终点记为 Finish）。
 *
 * 问总共有多少条不同的路径？
 *
 * 【输入格式】
 * 一行：两个整数 m 和 n，表示一个 m x n 网格。
 *
 * 【输出格式】
 * 一行一个整数，表示不同的路径条数。
 *
 * 【样例输入 1】
 * 3 7
 *
 * 【样例输出 1】
 * 28
 *
 * 【样例输入 2】
 * 3 2
 *
 * 【样例输出 2】
 * 3
 *
 * （从左上角开始，总共有 3 条路径可以到达右下角：
 * 向右->向下->向下；向下->向下->向右；向下->向右->向下）
 *
 * 【样例输入 3】
 * 7 3
 *
 * 【样例输出 3】
 * 28
 *
 * 【样例输入 4】
 * 3 3
 *
 * 【样例输出 4】
 * 6
 *
 * 【数据范围】
 * 1 <= m, n <= 20
 * 题目数据保证答案小于等于 2 * 10^9
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int differentPath(vector<vector<int>>& map) {
    int m = map.size();
    int n = map[0].size();
    vector<vector<int>> dp(m, vector<int>(n, 1));
    for (int i = 1; i < m; ++i) {
      for (int j = 1; j < n; ++j) {
        dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
      }
    }
    return dp[m - 1][n - 1];
  }
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<vector<int>> map(m, vector<int>(n, 1));
  Solution sol;
  cout << sol.differentPath(map) << endl;
}