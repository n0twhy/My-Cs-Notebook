/*
题目内容
硬件 PCB 板上两个芯片之间需要布一条 I2C 链路，两个芯片分别位于左上角和右下角，PCB 走线仅能向下和向右移动，但是当前 PCB 上已经有一些器件或者干扰源，器件和干扰源都要绕开，给一个二维数组，0 表示可以布线，1 表示已有器件，2 表示开关电源，3 表示开孔，4 表示 GND，避开干扰信号衰减越小，而且转弯次数越少越好，需要你找到从芯片 A 到芯片 B 之间通路的最少转弯次数，实现布线方案预估，如果没有通路，直接返回 -1。

输入描述
第一行 2 个整型数据 m，n 分别表示行数和列数，超出边界值直接返回 -1

后面 m 行 n 列整型矩阵表示硬件 PCB 板上已有器件分布情况，
0 表示可以布线，
1 表示已有器件，
2 表示开关电源，
3 表示开孔，
4 表示 GND

0 < m, n <= 100

0 <= pcb[i][j] <= 4

输出描述
返回芯片 A 到芯片 B 之间通路的最少转弯次数

样例1
输入
4 4
0 2 0 4
0 1 3 0
0 1 0 0
1 0 0 0

输出
-1

说明
从 A 芯片到芯片 B 无通路可以到达，转弯次数 -1

样例2
输入
3 3
0 1 0
0 0 0
2 0 0

输出
2

说明
解释: 输入一个 3*3 的二维数组，0 代表通路，非 0 代表有器件或者干扰源，不可通过。从 A 到 B，有 3 条路径，按照数字下标为
(0,0)->(1,0)->(1,1)->(1,2)->(2,2)，转两次，
(0,0)->(1,0)->(1,1)->(2,1)->(2,2) 转弯 3 次，所以最少转弯次数为 2

样例3
输入
-2 2
0 0
0 0

输出
-1

说明
输入参数不满足要求，直接返回 -1
*/

#include <vector>
#include <bits/stdc++.h>
using namespace std;
enum Orientation {
  right = 0,
  down = 1
};

int dx[2] = {0, 1};
int dy[2] = {1, 0};

int main() {
  int m, n;
  cin >> m >> n;
  if (m < 1 || n < 1) {
    cout << -1 << endl;
    return 0;
  }
  vector<vector<int>> grid(m, vector<int>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
    }
  }
  int ans = 0x3f3f3f3f;
  vector<vector<int>> visited(m, vector<int>(n, 0x3f3f3f3f));
  auto dfs = [&](auto &self, int x, int y, int turn_cnt, int curr_ori) {
    if (x == m - 1 && y == n - 1) {
      ans = min(ans, turn_cnt);
      return;
    }

    for (int i = 0; i < 2; ++i) {
      int cx = x + dx[i];
      int cy = y + dy[i];

      if (cx >= 0 && cy >= 0 && cx < m && cy < n && grid[cx][cy] == 0) {
        int tmp_ori = curr_ori;
        int tmp_cnt = turn_cnt;
        if (i != curr_ori) {
          tmp_ori = i;
          tmp_cnt++;
        }
        if (tmp_cnt < visited[cx][cy]) self(self, cx, cy, tmp_cnt, tmp_ori);
      }
    }
  };
  dfs(dfs, 0, 0, 0, 0);
  dfs(dfs, 0, 0, 0, 1);
  if (ans == 0x3f3f3f3f) {
    cout << -1 << endl;
    return 0;
  }
  cout << ans << endl;
  
}