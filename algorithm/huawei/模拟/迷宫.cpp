/*
题目描述
小明有一个神奇的糖果迷宫，这个迷宫是一个矩形迷宫，包含了两行（行数为2）和 m 列（列数为 m）的格子。
每个格子中都有不同的分数，由二维数组 a[i][j] 表示，其中 i 表示行号，j 表示列号。

小A和小B从迷宫的左上角（位置 a[0][0]）出发，走到迷宫的右下角（位置 a[1][m-1]）。
在这个过程中，他只能进行向右或向下的移动，每到达一个格子，他们都会吃掉这个格子的糖果。

假设小A先走，他会获得吃掉路径中的糖果，然后小B开始走，他不能重复吃掉小A吃过的糖果。

小A的目标是要尽量减少小B吃掉的糖果总数，而小B则希望在小A走完后，自己能吃掉更多的糖果总数。

请你计算小B最多可以吃掉多少糖果。

输入格式
第一行一个整数 m，表示迷宫的列数。
接下来两行，每行 m 个整数，表示当前位置的糖果数。

输出格式
输出小B最多可以吃掉的糖果数。

Sample Input 1
4
1 5 2 7
5 3 4 1

Sample Output 1
8

说明
1 ≤ m ≤ 10^5
*/

// 你的代码从这里开始写
#include <bits/stdc++.h>
#include <climits>
#include <iterator>
using namespace std;

class Solution {
public:
  int candies(vector<vector<int>> &maze) {
    int size = maze[0].size();

    vector<int> sum_1(size);
    vector<int> sum_2(size);
    int tmp_sum1 = 0;
    int tmp_sum2 = 0;
    
    for (int i = 0; i < size; ++i) {
      tmp_sum1 += maze[0][i];
      tmp_sum2 += maze[1][i];

      sum_1[i] = tmp_sum1;
      sum_2[i] = tmp_sum2;
    }

    int min_candies = INT_MAX;

    for (int i = 0; i < size; ++i) {
      int cur_max = max(sum_1[size - 1] - sum_1[i], sum_2[i] - maze[1][i]);
      min_candies = min(min_candies, cur_max);
    }

    return min_candies;
  }
};

int main() {
  int n;
  cin >> n;
  vector<vector<int>> maze(2, vector<int>(n));

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> maze[i][j];
    }
  }

  Solution sol;
  cout << sol.candies(maze) << endl;
}

