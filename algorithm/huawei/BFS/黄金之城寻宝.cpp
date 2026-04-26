/*
题目内容

小明是一名勇敢的冒险家，他一直梦想着找到传说中的黄金之城。他听说在一个遥远的沙漠中，有一个隐藏着无数宝藏的迷宫，只有最聪明和最勇敢的人才能进入并找到出路。小明决定去挑战这个迷宫，他带着一张地图和一些装备，踏上了寻宝之旅。

但是，这个迷宫并不是那么容易通过的，它充满了各种危险和难题。除了一些隐藏在地面上的陷阱之外，还有一些时隐时现的墙壁，它们会随机地出现和消失，阻挡小明的前进。小明必须小心地观察墙壁的状态循环，绕过陷阱和墙壁，或者等待合适的时机通过。

这是个 n*n 大小的迷宫，迷宫中存在着 k 个陷阱，并且每个位置都存在着一个墙壁的状态循环，状态循环以 3 个单位时间作为一个循环，0 表示没有墙壁，1 表示有墙壁。

小明在每个单位时间可以向上、下、左、右某个方向移动一个地图单位，当然也可以选择原地踏步。

限制：
如果小明移动方向上有陷阱，或者小明移动目的地在下一个单位时间出现墙壁，则不可以朝该方向移动；
同时，如果小明当前位置在下一个单位时间会出现墙壁，那小明也不可以选择停在原地。

我们需要计算出小明找到宝藏的最短时间。注意，小明可能并到不了目的地哦，这种情况输出 -1。

输入描述

输入第一行为一个整数 n，表示迷宫的大小。（2<=n<=100）

输入第二行为一个整数 k，表示迷宫中陷阱的数量。（0<k<=n*n-2）

接下来输入一行 2*k 个整数，具体为
row1 col1 row2 col2 ... rowk colk，
表示位置 (rowi, coli) 存在一个陷阱。

接下来一行为两对整数（row1, col1）和（row2, col2），表示宝藏的位置和小明的起始位置。

然后接下来 n 行：每行 n 个字符串空格分开，每个字符串长度固定为 3，内容固定只有 0 和 1，
表示每个位置的墙壁的状态循环。

注意：地图左上角为 (0,0)，输入保证所有位置合法。

输出描述

输出一个整数，表示小明找到宝藏的最短时间。

样例

样例一
输入
3
2
1 0 1 2
2 1 2 0
100 100 100
100 000 100
000 000 001

输出
1

样例解释
小明最快的移动顺序：[2,0] -> [2,1]

样例二
输入
3
2
1 0 2 0
0 1 2 2
000 000 001
010 101 101
110 010 000

输出
5

样例解释
小明最快的移动顺序：[2,2] -> [1,2] -> [2,2] -> [2,1] -> [1,1] -> [0,1]
*/

#include <bits/stdc++.h>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<string>> maze(n, vector<string>(n));
  set<pair<int, int>> traps;
  
  for (int i = 0; i < m; ++i) {
    int first, second;
    cin >> first >>second;
    traps.insert({first, second});
  }

  pair<int, int> treasure;
  cin >> treasure.first >> treasure.second;
  pair<int, int> start;
  cin >> start.first >> start.second;

  for (int i = 0; i < n; ++i) {
    for (int j = 0 ; j < n; ++j) {
      cin >> maze[i][j];
    }
  }

  queue<pair<int, int>> q;
  q.push(start);

  int dx[5] = {-1, 1, 0, 0, 0};
  int dy[5] = {0, 0, -1, 1, 0};

  int curr_time = 0;
  vector<vector<vector<bool>>> visited(n, vector<vector<bool>>(n, vector<bool>(3, false)));

  while (!q.empty()) {
    int size = q.size();
    int curr_state = curr_time % 3;
    int next_state = (curr_time + 1) % 3;
    for (int i = 0; i < size; ++i) {
      auto [x, y] = q.front();
      q.pop();
      if (visited[x][y][curr_state]) continue;
      visited[x][y][curr_state] = true;
      if (treasure.first == x && treasure.second == y) {
        cout << curr_time << endl;
        return 0;
      }
      for (int j = 0; j < 5; ++j) {
        int cx = x + dx[j];
        int cy = y + dy[j];

        if (cx >= 0 && cx < n && cy >= 0 && cy < n && maze[cx][cy][next_state] == '0' && !traps.count({cx, cy})) {
          q.push({cx, cy});
        }
      }
    }
    curr_time++; 
  }
  cout << "-1" << endl;
  return 0;
}