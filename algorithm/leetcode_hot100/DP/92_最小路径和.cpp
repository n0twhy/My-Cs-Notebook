/*
 * LeetCode 64 - 最小路径和 (Minimum Path Sum)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个包含非负整数的 m x n 网格 grid，请找出一条从左上角到右下角的路径，
 * 使得路径上的数字总和为最小。
 *
 * 说明：每次只能向下或者向右移动一步。
 *
 * 【输入格式】
 * 输入一个 m x n 网格（每行空格分隔）。
 *
 * 【输出格式】
 * 一行一个整数，表示最小路径和。
 *
 * 【样例输入 1】
 * 1 3 1
 * 1 5 1
 * 4 2 1
 *
 * 【样例输出 1】
 * 7
 *
 * （因为路径 1->3->1->1->1 的总和最小）
 *
 * 【样例输入 2】
 * 1 2 3
 * 4 5 6
 *
 * 【样例输出 2】
 * 12
 *
 * 【数据范围】
 * m == grid.length
 * n == grid[i].length
 * 1 <= m, n <= 200
 * 0 <= grid[i][j] <= 200
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int minimalPath(vector<vector<int>> &map) {
    int m = map.size();
    int n = map[0].size();

    vector<vector<int>> dp(m, vector<int>(n));
    dp[0][0] = map[0][0];
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (i == 0) {
          if (j == 0) continue;
          dp[i][j] = map[i][j] + dp[i][j - 1];
        }
        
        if (j == 0) {
          if (i == 0) continue;
          dp[i][j] = dp[i - 1][j] + map[i][j];
        }
        if (i > 0 && j > 0) {
          dp[i][j] = min(map[i][j] + dp[i - 1][j], map[i][j] + dp[i][j - 1]);
        }
      }
    }

    return dp[m - 1][n - 1];
  }
};

int main() {
  vector<vector<int>> map;
  string line;
  while(getline(cin, line) && !line.empty()) {
    stringstream ss(line);
    int n;
    vector<int> tmp;
    while(ss >> n) {
      tmp.emplace_back(n);
    }
    map.emplace_back(tmp);
  }
  
  Solution sol;
  cout << sol.minimalPath(map) << endl;
}