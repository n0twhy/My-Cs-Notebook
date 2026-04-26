/*
题目内容
小明用 k×k 的二维矩阵 map[][] 表示三维空间中的一个地图，map[i][j] 表示位置 [i, j] 上的地形高度。玩家需控制游戏中的一个角色穿过这个地图，从矩阵左上角位置 (坐标 0,0) 进入，从矩阵右侧任意位置出去。

规则：
1. 角色在矩阵中只能向右或向下移动。
2. 如果相邻两个节点高度差大于 1，则角色不能移动过去（太高角色爬不上去，太低了就摔死了）。
3. 角色通过 (i, j) 地点时，会消耗 map[i][j] 体力值。

求最省体力值的路线所消耗的体力值。

输入描述
输入有多行，第 1 行为数组的行数 k（k <= 100），第 2 行至第 k+1 行为 k×k 矩阵，数组元素用空格分隔，0 <= map[i][j] <= 10。

例如：
3
1 2 3
5 5 5
7 7 7

输出描述
输出一行，一个整数，表示最省体力值的路线所消耗的体力值。

当不存在可行路径，返回 -1。

参数不合法时返回 -2。

样例1
输入
3
1 2 3
5 5 5
7 7 7
输出
6
说明
通过路径的矩阵坐标为 [0,0], [0,1], [0,2]，依次消耗了 1, 2, 3 体力值，共计 6。其他路径由于高度差无法通过，因此最优解是 6。

样例2
输入
3
1 2 4
6 6 6
8 8 8
输出
-1
说明
由于高度差，没有可达到右侧的路径，返回 -1。

样例3
输入
3
1 2 1
1 1 2
9 9 9
输出
4
说明
红色路线消耗体力值为 4，黄色路线消耗体力值为 5，因此最优解为 4。（当然还存在其他路线，但都不如红色路线体力值小，本示例主要解释路径最优。）
*/

#include <bits/stdc++.h>
#include <climits>
#include <functional>
#include <vector>
using namespace std;

class Solution {
public:
  int Consume(vector<vector<int>> &map) {
    int m = map.size();
    int n = map[0].size();
    int min_path = INT_MAX;
    int dx[2] = {1, 0};
    int dy[2] = {0 , 1};
    function<void(int, int, int)> dfs = [&](int x, int y, int curr){
      if (y == n - 1) {
        min_path = min(min_path, curr);
      }

      for (int i = 0; i < 2; ++i) {
        int cx = x + dx[i];
        int cy = y + dy[i];

        if (cx >= 0 && cy >= 0 && cx < m && cy << n && (abs(map[x][y] - map[cx][cy]) <= 1)) {
          dfs(cx, cy, curr + map[cx][cy]);
        }
      }
    };
    dfs(0, 0, map[0][0]);
    if (min_path == INT_MAX) return -1;
    return min_path;
  }
};

int main() {
  int n;
  cin >> n;
  vector<vector<int>> map(n, vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> map[i][j];
    }
  }
  Solution sol;
  cout << sol.Consume(map) << endl;
}