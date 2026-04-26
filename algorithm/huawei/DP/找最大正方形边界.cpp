/*
本题考察的 LeetCode 原题：
LeetCode 1139. 最大的以 1 为边界的正方形

题目描述
现在有一个二维数组来模拟一个黑白棋盘，将黑白棋子置于方格之中。
棋盘中每个位置都放入黑棋或白棋，分别对应数组每个元素取值只能为 1 或 0
（黑棋为 1，白棋为 0）。

现在需要找一个正方形边界，其每条边上的棋子都是黑棋（对应数组中元素值为 1），
且该正方形面积最大。

输入描述
输入第一行为棋盘的行数（二维数组的行数）。
输入第二行为棋盘的列数（二维数组的列数）。
紧接着为模拟棋盘的二维数组 arr。

参数范围：
1 < arr.length <= 200
1 < arr[0].length <= 200

输出描述
返回满足条件的面积最大正方形边界信息，返回信息 [r,c,w]：
- r、c 分别代表方阵右下角的行号和列号；
- w 代表正方形的宽度。

如果存在多个满足条件的正方形，则返回 r 最小的；
若 r 相同，返回 c 最小的正方形。

样例1
输入
4
5
1 0 0 0 1
1 1 1 1 1
1 0 1 1 0
1 1 1 1 1

输出
[3,2,3]

说明
满足条件且面积最大的正方形边界，其右下角的顶点为 [3,2]，
即行号为 3，列号为 2，其宽度为 3，因此返回 [3,2,3]。

样例2
输入
3
3
1 0 0
0 1 0
0 0 1

输出
[0,0,1]

说明
满足条件且面积最大的正方形边界有三个，即 [0,0,1]、[1,1,1]、[2,2,1]。
根据要求，如果满足条件有多个，则返回 r 最小的，即 [0,0,1]。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> MaxSquare(vector<vector<int>> &grid) {
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> left_ones(m, vector<int>(n, 0));
    vector<vector<int>> up_ones(m, vector<int>(n, 0));

    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (grid[i][j] == 1) {
          left_ones[i][j] = j > 0 ? left_ones[i][j - 1] + 1 : 1;
          up_ones[i][j] = i > 0 ? up_ones[i - 1][j] + 1 : 1;
        }
      }
    }

    vector<vector<int>> dp(m, vector<int>(n, 0));
    vector<int> res(3, INT_MIN);
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        int edge = min(up_ones[i][j], left_ones[i][j]);
        
        if (i - edge + 1 < m && j - edge + 1 < n && left_ones[i - edge + 1][j] >= edge && up_ones[i][j - edge + 1] >= edge) {
          if (edge > res[2]) {
            res[0] = i;
            res[1] = j;
            res[2] = edge;
          }
        }
      }
    }
    return res;
  }    
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<vector<int>> grid(m, vector<int>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
    }
  }

  Solution sol;
  auto res = sol.MaxSquare(grid);
  cout << "[" << res[0] << "," << res[1] << "," << res[2] << "]";
}