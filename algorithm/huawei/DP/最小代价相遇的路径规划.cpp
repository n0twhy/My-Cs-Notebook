/*
题目：最小代价相遇的路径规划

题目内容
给定一个 n×n 的非负整数矩阵地图 grid，地图左上角为 [0,0]。
有两辆车分别从左上角 [0,0] 和右下角 [n-1,n-1] 出发，进行货物交接。

每辆车可以从一个坐标移动到相邻坐标（上下左右），经过的每个位置都会产生代价，
且路径代价包含起始位置的代价。其中 grid[i][j] 表示通过位置 [i,j] 的代价。
若 grid[i][j] = 0，表示该位置为障碍物，车辆无法通过。

两辆车相遇的定义：
两辆车最终分别停在上下或左右相邻的两个网格位置，并且路径可达。

两辆车相遇的代价定义：
两辆车到达各自相遇位置所需代价中的较大值。

注意：
行驶过程中车辆可以停在某个网格位置，两辆车无需同步行驶。

在本题输入约束下：
- 左上角出发的车只能向右或向下移动；
- 右下角出发的车只能向上或向左移动。

求两辆车可以相遇的最小代价；若无法相遇则返回 -1。

输入描述
第一行输入一个整数 n，表示地图大小为 n 行 n 列。
后续输入 n 行，每行 n 个整数，表示 grid 的每一行元素值。

参数范围：
2 <= n <= 1000
0 <= grid[i][j] <= 100
grid[0][0] != 0
grid[n-1][n-1] != 0

输出描述
输出一个整数，表示两辆车相遇的最小代价；
如果两车无法相遇（例如都被障碍物阻挡），返回 -1。

样例1
输入
2
1 2
2 1

输出
3

说明
地图为：
[
 [1,2],
 [2,1]
]

一种最优方案：
第一辆车路径：[0,0] -> [0,1]，代价 1+2=3
第二辆车路径：[1,1]，代价 1
两车停在相邻位置 [0,1] 与 [1,1]，相遇代价取较大值 3。

样例2
输入
3
1 2 3
1 2 3
1 2 3

输出
5

说明
地图为：
[
 [1,2,3],
 [1,2,3],
 [1,2,3]
]

一种最优方案：
第一辆车路径：[0,0] -> [1,0] -> [2,0]，代价 1+1+1=3
第二辆车路径：[2,2] -> [2,1]，代价 3+2=5
两车停在相邻位置 [2,0] 与 [2,1]，相遇代价取较大值 5。

样例3
输入
3
1 0 3
1 0 3
1 0 3

输出
-1

说明
地图中存在障碍物阻断路径，两辆车无法相遇，返回 -1。
*/
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n; cin >> n;
  // 1-indexed,哨兵行列默认 0(障碍)
  vector<vector<int>> g(n+2, vector<int>(n+2, 0));
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
      cin >> g[i][j];

  const int INF = 0x3f3f3f3f;
  vector<vector<int>> dp1(n+2, vector<int>(n+2, INF));
  vector<vector<int>> dp2(n+2, vector<int>(n+2, INF));

  // 车1:从 (1,1) 向右/向下
  dp1[1][1] = g[1][1];
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (i == 1 && j == 1) continue;
      if (g[i][j] == 0) continue;
      int up   = (i > 1) ? dp1[i-1][j] : INF;
      int left = (j > 1) ? dp1[i][j-1] : INF;
      if (up == INF && left == INF) continue;
      dp1[i][j] = min(up, left) + g[i][j];
    }
  }

  // 车2:从 (n,n) 向上/向左
  dp2[n][n] = g[n][n];
  for (int i = n; i >= 1; i--) {
    for (int j = n; j >= 1; j--) {
      if (i == n && j == n) continue;
      if (g[i][j] == 0) continue;
      int down  = (i < n) ? dp2[i+1][j] : INF;
      int right = (j < n) ? dp2[i][j+1] : INF;
      if (down == INF && right == INF) continue;
      dp2[i][j] = min(down, right) + g[i][j];
    }
  }

  int ans = INF;
  auto upd = [&](int a, int b) {
    if (a != INF && b != INF) ans = min(ans, max(a, b));
  };
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      // 四种相遇:车1 在 (i,j) 且车2 在右/下邻;反过来也一样
      upd(dp1[i][j], dp2[i][j+1]);
      upd(dp1[i][j], dp2[i+1][j]);
      upd(dp2[i][j], dp1[i][j+1]);
      upd(dp2[i][j], dp1[i+1][j]);
    }
  }
  cout << (ans == INF ? -1 : ans) << endl;
}