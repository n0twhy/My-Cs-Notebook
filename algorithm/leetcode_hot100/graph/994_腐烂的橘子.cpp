/*
力扣 994. 腐烂的橘子
https://leetcode.cn/problems/rotting-oranges/

题目描述：
在给定的 m x n 网格 grid 中，每个单元格可以有以下三个值之一：
- 值 0 代表空单元格；
- 值 1 代表新鲜橘子；
- 值 2 代表腐烂的橘子。

每分钟，腐烂的橘子在上下左右四个方向上相邻的新鲜橘子都会腐烂。

返回直到单元格中没有新鲜橘子为止所必须经过的最小分钟数。如果不可能，返回 -1。

示例 1：
输入：grid = [[2,1,1],[1,1,0],[0,1,1]]
输出：4

示例 2：
输入：grid = [[2,1,1],[0,1,1],[1,0,1]]
输出：-1
解释：左下角的橘子（第 2 行，第 0 列）永远不会腐烂，因为腐烂只会发生在 4 个方向上。

示例 3：
输入：grid = [[0,2]]
输出：0
解释：因为 0 分钟时已经没有新鲜橘子了，所以答案就是 0。

提示：
- m == grid.length
- n == grid[i].length
- 1 <= m, n <= 10
- grid[i][j] 仅为 0、1 或 2
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int RottenOrange(vector<vector<int>> &grid) {
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    int m = grid.size();
    int n = grid[0].size();

    int fresh = 0;
    queue<pair<int, int>> q;
    int time = 0;

    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (grid[i][j] == 1) fresh++;
        else if (grid[i][j] == 2) q.push({i, j});
      }
    }

    while (!q.empty()) {
      int size = q.size();
      bool has_rotten = false;
      for (int i = 0; i < size; ++i) {
        auto [x, y] = q.front();
        q.pop();

        for (int j = 0; j < 4; ++j) {
          int cx = x + dx[j];
          int cy = y + dy[j];

          if (cx >= 0 && cx < m && cy >= 0 && cy < n && grid[cx][cy] == 1) {
            has_rotten = true;
            q.push({cx, cy});
            grid[cx][cy] = 2;
            fresh--;
          }
        }

      }

      if (has_rotten) time++;
    }
    if (fresh > 0) return -1;
    return time;
  }
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
  cout << sol.RottenOrange(grid) << endl;
}