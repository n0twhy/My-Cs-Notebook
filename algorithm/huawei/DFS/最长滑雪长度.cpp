/*
题目：最长滑雪长度（欢乐滑雪）

题目内容
某手机应用市场下载量很高的游戏《欢乐滑雪》会自动生成一张 m × n 格子的地形图，每个单元格标明高度，
高度为 [1, 2147483647]（即 32 位有符号整数上界 0x7FFFFFFF）中的整数。

玩家可任选一格作为起点，沿上、下、左、右四个方向滑动；不能斜走、不能出界，且已走过的格子不能重复。

规则：只能从高往低滑——下一步格子的高度必须**严格小于**当前格子高度。
求在满足规则的前提下，能走过的最长路径长度（经过的格子个数）。

约束
1 <= m, n <= 500
1 <= matrix[i][j] <= 2147483647
0 <= i <= m - 1，0 <= j <= n - 1

输入描述
第一行：m n（[1,500]）。

接下来 m 行，每行 n 个整数，表示各行格子高度。

输出描述
输出一条合法最长路径的长度（格子数）。

样例1
输入：
3 3
9 8 7
6 6 6
6 6 6

输出：
4

说明：一条最长路径可为 9 -> 8 -> 7 -> 6，长度为 4。到达高度 6 后，不能进入其它同样为 6 的格子（必须严格下降）。

样例2
输入：
1 5
10 11 12 13 14

输出：
5

说明：单行时可沿 14 -> 13 -> 12 -> 11 -> 10，长度为 5。
*/

#include <bits/stdc++.h>
#include <climits>
#include <vector>
using namespace std;

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

class Solution {
public:
  int MaximalLength(vector<vector<int>> &instance) {
    int res = INT_MIN;
    int m = instance.size(), n = instance[0].size();
    function<void(int, int, int, int, int&)> dfs = [&](int x, int y, int curr_length, int curr_height, int &res) {
      res = max(res, curr_length);
      for (int i = 0; i < 4; ++i) {
        int cx = x + dx[i];
        int cy = y + dy[i];

        if (cx >= 0 && cx < m && cy >= 0 && cy < n && instance[cx][cy] > curr_height ) {
          int height_backup = instance[cx][cy];
          instance[cx][cy] = INT_MIN;
          dfs(cx, cy, curr_length + 1, height_backup, res);
          instance[cx][cy] = height_backup;
        }
      }
    };
    int ans = INT_MIN;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        int curr_res = 0;
        dfs(i, j, 1, instance[i][j], curr_res);
        ans = max(ans, curr_res);
      }
    }
    return ans;
  }
};
int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<int>> instance(m, vector<int>(n));
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        cin >> instance[i][j];
      }
    }
    Solution sol;
    cout << sol.MaximalLength(instance) << endl;
}
