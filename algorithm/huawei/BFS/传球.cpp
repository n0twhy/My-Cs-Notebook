/*
题目描述

有一个 n*m 的 01 矩阵，你需要从第一列的任意一个 1 出发，到达最后一列的任意一个 1。
途径的点必须为 1，求最少的步数。

输入描述

第一行两个整数 n,m，代表有一个 n 行 m 列的 01 矩阵。

接下来 n 行，每行 m 个数。每个数非 0 即 1。

输出描述

输出最短步数，当不可达时输出 -1。

样例
输入
3 4
0 1 1 0
1 1 1 1
0 1 0 1

输出
3
*/

#include <bits/stdc++.h>
#include <queue>
#include <vector>
using namespace std;

int main() {
  int m, n;
  cin >> m >> n;
  queue<pair<int, int>> q;
  set<pair<int, int>> set;
  vector<vector<int>> grid(m, vector<int>(n));
  vector<vector<bool>> visited(m, vector<bool>(n, false));

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
      if (j == 0 && grid[i][j]) {
        q.push({i, j});
        visited[i][j] = true;
      }
      if (j == n - 1 && grid[i][j]) set.insert({i, j});
    }
  }

  int length = 1;
  
  int dx[4] = {0, 0, 1, -1};
  int dy[4] = {1, -1, 0, 0};
  while (!q.empty()) {
    int size = q.size();
    for (int i = 0; i < size; ++i) {
      auto [x, y] = q.front();
      q.pop();
      for (int j = 0; j < 4; ++j) {
        int cx = x + dx[j];
        int cy = y + dy[j];

        if (cx >= 0 && cx < m && cy >= 0 && cy < n && grid[cx][cy] == 1) {
          if (visited[cx][cy]) continue;
          if (set.count({cx, cy})) {
            cout << length << endl;
            return 0;
          }
          visited[cx][cy] = true;
          q.push({cx, cy});
        }
      }
    }
    length++;
  }
  cout << -1 << endl;
}