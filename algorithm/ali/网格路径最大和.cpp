/*
题目内容
给定一个 2×n 的网格，记数组为 {a_{i,j}}。行与列均从 0 开始编号，其中 i ∈ {0,1}，j ∈ [0, n-1]。你可以进行如下操作任意次（包括 0 次）：

选择一个下标对 (i, j)，若 0 <= j xor 1 < n，则交换 a_{i,j} 与 a_{i xor 1, j xor 1}；否则该 (i, j) 不可被选择。

全部交换操作完成后，TK 初始位于 (0, 0)，每一步可以向下或者向右移动一格（不可移动到网格外），直到到达 (1, n-1)。移动过程中，TK 会将经过的所有单元格的值累加（包括起点与终点）。

你的目标是在进行若干次操作后，为 TK 选择一条合法路径，使得累加和最大，并输出这个最大值。

【名词解释】
按位异或：xor 表示按位异或运算。这里的 xor 1 表示将 x 的二进制最低位翻转（0 <-> 1）。

输入描述
每个测试文件均包含多组测试数据。第一行输入一个整数 t(1 <= t <= 10^4) 表示数据组数。每组测试数据描述如下：

第一行输入一个整数 n(1 <= n <= 2 * 10^5) 表示网格的列数；

接下来两行每行输入 n 个整数，分别为 a_{0,0}, a_{0,1}, ..., a_{0,n-1} 与 a_{1,0}, a_{1,1}, ..., a_{1,n-1}(1 <= a_{i,j} <= 10^9)，

除此之外，保证单个测试文件的 n 之和不超过 2 * 10^5。

输出描述
对于每一组测试数据，新起一行，输出一个整数表示最大值。

样例1
输入
2
2
1 2
3 4
3
5 1 7
3 10 4
输出
8
24

说明
样例解释（第二组）：

交换 a_{0,0} 与 a_{1,1}，再交换 a_{1,0} 与 a_{0,1}，得到顶行 {10,3,7}、底行 {1,5,4}；
选择路径 R->R->D，累加和为 10+3+7+4=24，为最大值。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;
#define int long long
class Solution {
public:
  int MaximalSum() {
    int n;
    cin >> n;
    vector<vector<int>> grid(2, vector<int>(n));
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < n; ++j) {
        cin >> grid[i][j];
      }
    }

    int pass = 0;

    for (int i = 0; i < n; i += 2) {
      if (i == n - 1) {
        pass += grid[1][i];
        break;
      }

      pass += max(grid[0][i], grid[1][i + 1]) + max(grid[0][i + 1], grid[1][i]);
    }

    int bonus = 0;

    for (int i = 0; i < n; i += 2) {
      if (i == n - 1) {
        bonus = max(bonus, grid[0][i]);
        break;
      }
      bonus = max(bonus, min(grid[0][i], grid[1][i + 1]));
    }
    return pass + bonus;
  }
};

signed main() {
  int T;
  cin >> T;
  Solution sol;
  for (int i = 0; i < T; ++i) {
    cout << sol.MaximalSum() << endl;
  }
}