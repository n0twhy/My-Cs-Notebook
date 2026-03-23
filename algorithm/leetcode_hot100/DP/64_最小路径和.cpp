/*
 * LeetCode 64 - 最小路径和 (Minimum Path Sum)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个 m×n 的非负整数网格 grid，从左上角走到右下角，每次只能向右或向下移动一步。
 * 求路径上数字之和的最小值。
 *
 * 【输入格式】
 * 第一行：两个整数 m、n，表示网格行数和列数。
 * 接下来 m 行：每行 n 个非负整数，表示该行格子中的值，空格分隔。
 *
 * 【输出格式】
 * 一行一个整数，表示从 (0,0) 到 (m-1,n-1) 的最小路径和。
 *
 * 【样例输入 1】
 * 3 3
 * 1 3 1
 * 1 5 1
 * 4 2 1
 *
 * 【样例输出 1】
 * 7
 *
 * （路径 1→3→1→1→1 和为 7）
 *
 * 【样例输入 2】
 * 2 2
 * 1 2
 * 3 4
 *
 * 【样例输出 2】
 * 7
 *
 * 【数据范围】
 * m、n 属于 [1, 200]
 * grid[i][j] 属于 [0, 100]
 *
 * =============================================================================
 */
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
  int MinimalPath(vector<vector<int>>& grid) {
    dp.resize(grid.size(), vector<int>(grid[0].size()));
    dp[0][0] = grid[0][0];
    for (int i = 0; i < grid.size(); ++i) {
      for (int j = 0; j < grid[0].size(); ++j) {
        if (i == 0 && j > 0) {
          dp[i][j] = dp[i][j - 1] + grid[i][j];
        } else if (i > 0 && j == 0) {
          dp[i][j] = dp[i - 1][j] + grid[i][j]; 
        } else if (i > 0 && j > 0) {
          dp[i][j] = min(dp[i - 1][j] + grid[i][j], dp[i][j - 1] + grid[i][j]);
        }
      }
    }
    return dp[grid.size() - 1][grid[0].size() - 1];
  }

private:
  vector<vector<int>> dp;
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<vector<int>> grid(m, vector<int>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
    }
  }
  Solution sol;
  int res = sol.MinimalPath(grid);
  cout << res << endl;
}