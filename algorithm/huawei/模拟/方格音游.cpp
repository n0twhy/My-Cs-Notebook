/*
题目：方格音游

题目描述
小明最近沉迷于设计一个二维平面音游。

在示例中，假设有 24 个方块，分为水平 8 个和垂直 3 个。

当玩家点击一个亮起的方块时，它周围的所有方块都会高亮。为实现这个互动功能，
小明提出了一个“方块组”的概念：一个方块的方块组是指在上下左右方向上紧挨着的
所有方块和它自己。而且垂直方向上的方块是首尾相连的，但水平方向上的方块首尾不连。

例如，在 24 个方块中：
- 方块 0 的方块组是 “16 8 1 0”
- 方块 10 的方块组是 “2 9 18 11 10”
- 方块 23 的方块组是 “15 22 7 23”

为了方便理解，将最后一行方块复制一份到第一行，表示上下相连：
16 17 18 19 20 21 22 23
0  1  2  3  4  5  6  7
8  9  10 11 12 13 14 15
16 17 18 19 20 21 22 23

小明想要当玩家点击某个方块时，这个方块的整个方块组都会亮起。请设计程序，
能够实时得到某个方块的方块组。

输入描述
第一行输入为 3 个正整数 H、V、M：
- 1 <= H, V <= 256
- 1 <= M < H * V
其中 H、V 分别表示方块盘的水平个数和垂直个数。

接下来 M 行，每一行输入一个方块 ID。
方格 ID 从 0 开始，不超过 H * V - 1。

例如：
8 3 1
12

输出描述
输出对应的方格组，方格组内 ID 以空格分隔，
ID 从上方起始，逆时针排列，输入的方格 ID 放最后。

如上例输出为：4 11 20 13 12

样例1
输入：
12 4 1
21

输出：
9 20 33 22 21

样例2
输入：
16 4 2
0
21

输出：
48 16 1 0
5 20 37 22 21
*/

#include <bits/stdc++.h>
#include <functional>
using namespace std;

class Solution {
public:
  vector<int> CubeGame(vector<vector<int>> &board, int cube_id) {
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, -1, 0, 1};

    vector<int> res;
    int m = board.size();
    int n = board[0].size();
    
    int x = cube_id / n;
    int y = cube_id % n;

    for (int i = 0; i < 4; ++i) {
      int cx = x + dx[i];
      int cy = y + dy[i];

      if (cx >= -1 && cx <= m && cy >= 0 && cy < n) {
        if (cx == -1) {
          res.push_back((m - 1) * n + cy);
        } else if (cx == m) {
          res.push_back(cy);
        } else {
          res.push_back(cx * n + cy);
        }
      }
    }
    res.push_back(cube_id);
    return res;
  }
};

int main() {
  int h, w, m;
  cin >> h >> w >> m;
  vector<vector<int>> board(w, vector<int>(h));
  Solution sol;
  for (int i = 0; i < m; ++i) {
    int cube_id;
    cin >> cube_id;
    vector<int> tmp = sol.CubeGame(board, cube_id);
    for (int i = 0; i < tmp.size(); ++i) {
      cout << tmp[i] << " ";
    } 
    cout << endl;
  }
}
