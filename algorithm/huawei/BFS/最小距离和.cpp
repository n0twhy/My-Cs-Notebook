/*
题目内容

每天早晨，环卫工人需要处理各个小区的生活垃圾，每个小区的生活垃圾由一队环卫工人负责运送到最近的垃圾回收站进行处理，求将所有小区垃圾送到垃圾回收站的最小距离和。

假设小区和垃圾回收站都在一个 m 行 x n 列的区域矩阵上，相邻点的距离为 1，只能上下左右移动；
其中 0 表示垃圾处理站，1 表示小区，2 表示空白区域，-1 表示障碍区域不可通行。

区域内如果没有小区或者没有垃圾回收站，则最小距离和返回 0。
无法到达垃圾回收站的小区会单独处理，不计入本次距离和中。

计算所有小区垃圾送到垃圾回收站的最小距离和。

输入描述

第一行为两个数字 m 和 n，表示区域矩阵的行数和列数，中间使用空格分隔，
m 和 n 的范围均为 [1,300]。

接下来的 m 行表示一个 m×n 的区域矩阵数组，每行元素间以空格分隔，
其中元素取值仅为 -1（障碍区域）、0（垃圾处理站）、1（小区）、2（空白区域）。

输出描述

一个整数，表示所计算的最小距离和。

样例1
输入
4 4
1 2 -1 1
2 0 2 0
2 2 -1 2
1 2 1 1

输出
11

说明
如图所示，位置 [0,0]、[0,3]、[3,0]、[3,2]、[3,3] 是小区，
位置 [1,1]、[1,3] 是垃圾站，位置 [0,2]、[2,2] 是障碍，无法通行。
5 个小区，2 个垃圾站，小区到垃圾站的最小路径是 2+3+1+3+2=11。

对于位置 [3,2] 的小区，可以将垃圾运送到垃圾站 [1,1]、[1,3]，两者距离相同。
题解图示仅以到 [1,3] 垃圾站进行说明。

样例2
输入
2 3
0 -1 1
1 -1 2

输出
1

说明
如图所示，位置 [0,2]、[1,0] 是小区，位置 [0,0] 是垃圾站，
位置 [0,1]、[1,1] 是障碍，无法通行。2 个小区，1 个垃圾站，
小区到垃圾站的最小路径是 1+0=1。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> grid(n, vector<int>(m));
  queue<pair<int, int>> q;
  vector<vector<bool>> visited(n, vector<bool>(m, false));

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cin >> grid[i][j];
      if (grid[i][j] == 0) {
        q.push({i, j});
        visited[i][j] = true;
      }
    }
  }

  int dx[4] = {-1, 1, 0, 0};
  int dy[4] = {0, 0, -1, 1};

  int curr_length = 1;
  int res = 0;
  
  while (!q.empty()) {
    int size = q.size();

    for (int i = 0; i < size; ++i) {
      auto [x, y] = q.front();
      q.pop();

      for (int j = 0; j < 4; ++j) {
        int cx = x + dx[j];
        int cy = y + dy[j];

        if (cx >= 0 && cx < n && cy >=0 && cy < m && grid[cx][cy] != -1) {
          if (visited[cx][cy]) continue;
          if (grid[cx][cy] == 1) res += curr_length;
          visited[cx][cy] = true;
          q.push({cx, cy});
        }
      }
    }
    curr_length++;
  }
  cout << res << endl;
}
