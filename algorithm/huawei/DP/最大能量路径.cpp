/*
题目内容
在自动驾驶系统中，车道线识别是核心功能之一。车道线通常具有连续性，从图像左侧到右侧逐渐展开。

为了识别出最可能的车道线路径，我们可以在图像中找到一条路径，使得路径上所有像素的信号值与策略矩阵的乘积之和最大。

现定义每个位置的能量值为策略矩阵与该位置周边信号值的乘积和。

给定一个 H×W 的图像以及一个 K×K 的策略矩阵，用于模拟不同方向的路径选择策略。

你需要从图像的第一列任意像素出发，走到最后一列任意像素，每一步只能向右、右上、右下移动一格。

在行进的过程中，需要实时的收集能量值，请找到一条路径，使得路径上的能量值之和最大。

输入描述
第一行输入 H W K K，分别表示给定图像及策略矩阵的维度

接下来 H 行输入图像矩阵

接下来 K 行输入策略矩阵

输出描述
输出最大能量值

样例1
输入
1 1 1 1
5
1
输出
5.0
说明
有且仅有一条路径，最大能量值为 5*1 为 5.0

样例2
输入
3 3 3 3
1 2 3
4 5 6
7 8 9
1 2 2
1 1 1
1 1 1
输出
119.0
说明
输入第一行是一个 3×3 的图像以及 3×3 的策略矩阵

每个位置的能量图：
[[12, 21, 16],
 [30, 50, 36],
 [33, 50, 34]]

最大能量路径的值：119.0
最大能量路径：(2,0)->(1,1)->(1,2)

提示
1. 策略矩阵为奇数，边缘处用零填充
2. 输出保留一位小数
*/

#include <bits/stdc++.h>
#include <cerrno>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  int maxenergy(vector<vector<int>> &graph, vector<vector<int>> &strategy) {
    int m = graph.size();
    int n = graph[0].size();
    int k = strategy.size();

    int mid = k / 2;

    function<int(int, int)> countenergy = [&](int x, int y) -> int {
      int energy = 0;
      for (int i = -mid; i <= mid; ++i) {
        for (int j = -mid; j <= mid; ++j) {
          int x_g = x + i, y_g = y + j;
          int x_s = mid + i, y_s = mid + j;
          if (x_g >= 0 && x_g < m && y_g >= 0 && y_g < n
            && x_s >= 0 && x_s < k && y_s >= 0 && y_s < k) {
              energy += graph[x_g][y_g] * strategy[x_s][y_s];
          } 
        }
      }
      return energy;  
    };
    vector<vector<int>> energy_graph(m, vector<int>(n));
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        energy_graph[i][j] = countenergy(i, j);
      }
    }

    vector<vector<int>> dp(m, vector<int>(n));
    for (int i = 0; i < m; ++i) {
      dp[i][0] = energy_graph[i][0];
    }
    int max_energy = 0;
    for (int j = 1; j < n; ++j) {
      for (int i = 0; i < m; ++i) {
        int upleft = i - 1 >= 0 ? dp[i - 1][j - 1] : 0;
        int left = dp[i][j - 1];
        int downright = i + 1 < m ? dp[i + 1][j - 1] : 0;
        dp[i][j] = energy_graph[i][j] + max({upleft, left, downright});
        if (j == n - 1) {
          max_energy = max(max_energy, dp[i][j]);
        }
      }
    }
    return max_energy;
  }
};

int main() {
  int h, w, k, k_1;
  cin >> h >> w >> k >> k_1;
  vector<vector<int>> graph(h, vector<int>(w));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      cin >> graph[i][j];
    }
  }
  vector<vector<int>> strategy(k, vector<int>(k));
  for (int i = 0; i < k; ++i) {
    for (int j = 0; j < k; ++j) {
      cin >> strategy[i][j];
    }
  }

  Solution sol;
  cout << fixed << setprecision(1) << (double)sol.maxenergy(graph, strategy) << endl;
}