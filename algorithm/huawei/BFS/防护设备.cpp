/*
题目内容

有一个 N×N 大小的迷宫。初始状态下，配送员位于迷宫的左上角，他希望前往迷宫的右下角。
配送员只能沿着上下左右四个方向移动，从每个格子移动到相邻格子所需要的时间是 1 个单位，
他必须用最多 K 个（也可以少于 K 个）单位时间到达右下角格子。

迷宫的每个格子都有辐射值，配送员必须穿着防护能力不低于相应辐射值的防护服，才能通过该格子。
他希望知道，防护服的防护能力最少要达到多少，才能顺利完成任务。

注意：配送员需要通过迷宫的左上角和右下角，因此防护服的防护能力必须大于等于这两个格子的辐射值。

输入描述

前两行各包含一个正整数，分别对应 N 和 K。
后 N 行各包含 N 个整数，以空格分隔，表示地图上每个位置的辐射值。

2<=N<=100，K>=2N-2（以保证题目有解）。
所有辐射值都是非负整数，绝对值不超过 10^4。

输出描述

一个整数，表示配送员穿着防护服的最低防护能力。

样例1
输入
2
2
1 3
2 1

输出
2

说明
配送员可以选择通过左下角（辐射值为 2）的路线，耗费 2 单位时间。

样例2
输入
5
12
0 0 0 0 0
9 9 3 9 0
0 0 0 0 0
0 9 5 9 9
0 0 0 0 0

输出
3

说明
最优路线：往右 2 格，往下 2 格，往左 2 格，往下 2 格，往右 4 格，耗费 12 单位时间，
经过格子的最大辐射值为 3。
另外，在地图不变的情况下，如果 K=16，输出为 0；如果 K=8，输出为 5。
*/

#include <bits/stdc++.h>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int CanIGoThereInK(int defend, vector<vector<int>> &grid) {
  int n = grid[0].size();
  vector<vector<bool>> visited(n, vector<bool>(n, false));
  queue<pair<int, int>> q;
  q.push({0, 0});
  visited[0][0] = true;
  int curr_path = 0;

  while (!q.empty()) {
    int size = q.size(); 
    for (int j = 0; j < size; ++j) {
      auto [x, y] = q.front();
      q.pop();
      for (int i = 0; i < 4; ++i) {
        int cx = x + dx[i];
        int cy = y + dy[i];

        if (cx >= 0 && cx < n && cy >= 0 && cy < n && grid[cx][cy] <= defend) {
          if (visited[cx][cy]) continue;
          if (cx == n - 1 && cy == n - 1) return curr_path;
          q.push({cx, cy});
          visited[cx][cy] = true;
        }
      }
    }
    curr_path++;
  }
  return -1;
}

int main() {
  int n, k;
  cin >> n >> k;
  int right = -1;
  vector<vector<int>> grid(n, vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
      right = max(right, grid[i][j]);
    }
  }
  int left = max(grid[0][0], grid[n - 1][n - 1]);
  int res = 0x3f3f3f3f;
  while (left <= right) {
    int mid = (left + right) / 2;
    int curr = CanIGoThereInK(mid, grid);

    if (curr > k || curr == -1) {
      left = mid + 1;
      continue;
    } else {
      res = min(res, mid);
      right = mid - 1;
      continue;
    }
  }
  cout << res << endl;
  
}