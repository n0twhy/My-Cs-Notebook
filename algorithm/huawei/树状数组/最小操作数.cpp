/*
题目：最小操作数

题目内容
给定一个 N×N 的二维矩阵，其中包含 [1, N^2] 的互不相同正整数。

定义一种操作：
每次可以选择矩阵中的一个元素，将其与其在“顺时针螺旋顺序”中的下一个元素交换位置。

例如在 3×3 矩阵中，螺旋顺序为：
[0,0] -> [0,1] -> [0,2] -> [1,2] -> [2,2] -> [2,1] -> [2,0] -> [1,0] -> [1,1]。

目标是通过若干次操作，使矩阵变为“顺时针螺旋递增”顺序，
即按螺旋遍历时元素依次为 1,2,3,...,N^2。

求将给定矩阵转换为目标状态所需的最小操作次数。

输入描述
第一行输入整数 N。
接下来 N 行，每行 N 个整数，表示矩阵。

参数范围：
1 <= N <= 10^3
矩阵元素范围为 [1, N^2]

输出描述
输出一个整数，表示最小操作次数。

特别注意：
结果可能很大，需要对 1000000007 取模。
例如初始结果为 1000000008 时，输出 1。

样例1
输入
2
3 1
2 4

输出
3

说明
目标矩阵为：
1 2
4 3

一种最优交换过程（每次交换螺旋序相邻元素）：
1) 交换 (0,0)=3 与 (0,1)=1：
   1 3
   2 4
2) 交换 (1,1)=4 与 (1,0)=2：
   1 3
   4 2
3) 交换 (0,1)=3 与 (1,1)=2：
   1 2
   4 3

共 3 次。

样例2
输入
3
3 2 1
6 5 4
9 8 7

输出
10

说明
目标矩阵为：
1 2 3
8 9 4
7 6 5

按螺旋顺序相邻交换，最少需要 10 次可达目标。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;
#define int long long

class Solution {
private:
   int lowbit(int x) {
      return x & (-x);
   }

   void update(vector<int> &tree, int num, int add) {
      while (num < tree.size()) {
         tree[num] += add;
         num += lowbit(num);
      }
   }

   int query(vector<int> &tree, int num) {
      int sum = 0;
      while (num > 0) {
         sum += tree[num];
         num -= lowbit(num);
      }
      return sum;
   }
public:
   int LeastOperateCnt(vector<vector<int>> &grid) {
      int MOD = 1000000007;
      int m = grid.size();
      int n = grid[0].size();
      
      int up = 0;
      int down = m -1;
      int left = 0;
      int right = n - 1;

      int res = 0;
      vector<int> tree(1e6 + 1, 0);
      while (left <= right && up <= down) {
         for (int y = left; y <= right; ++y) {
            res = (res + query(tree, 1e6) - query(tree, grid[up][y])) % MOD;
            update(tree, grid[up][y], 1);
         }
         up++;

         for (int x = up; x <= down; ++x) {
            res = (res + query(tree, 1e6) - query(tree, grid[x][right])) % MOD;
            update(tree, grid[x][right], 1);
         }
         right--;

         for (int y = right; y >= left; --y) {
            res = (res + query(tree, 1e6) - query(tree, grid[down][y])) % MOD;
            update(tree, grid[down][y], 1);
         }
         down--;

         for (int x = down; x >= up; --x) {
            res = (res + query(tree, 1e6) - query(tree, grid[x][left])) % MOD;
            update(tree, grid[x][left], 1);
         }
         left++;
      }
      return res;
   }
};

signed main() {
   int n;
   cin >> n;
   vector<vector<int>> grid(n, vector<int>(n));
   for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        cin >> grid[i][j];
      }
   }

   Solution sol;
   cout << sol.LeastOperateCnt(grid) << endl;
}