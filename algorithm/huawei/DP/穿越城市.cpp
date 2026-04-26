/*
题目：穿越城市

题目内容
小明需要走路从城市的一端前往另一端。城市可以视为一个长条形，共有 N 个街区，按顺序排成一列，
每个街区的右侧紧挨着下一个街区的左侧。

初始时，小明位于第 1 个街区的左侧，他的目标是到达第 N 个街区的右侧。
步行通过第 n 个街区时，小明需要花费的时间为 a_n。

同时，小明可以选择坐最多 M 次地铁。每个街区的左侧都有地铁站，
每次坐地铁可以穿越前方最少 1 个、最多 K 个连续的街区。

坐地铁穿越任何一个街区所需的时间都是一个常数 B
（如果穿越 2 个街区，所需时间是 2×B，以此类推），
进地铁站、出地铁站、等待地铁均不耗费时间。

输入描述
前两行各包含一个正整数，分别对应 N 和 K。

第三行包含 N 个非负整数，以空格分隔，对应于步行穿过每个街区所消耗的时间。

后两行各包含一个非负整数，分别对应 B 和 M。

参数范围：
1 <= N <= 10
1 <= K <= 10
0 <= M <= 10
以任何方式通过单个街区所需要的时间（包括所有 a_n 以及 B 的值）不超过 10^4。

输出描述
一个整数，表示穿越整个城市花费的最短时间。

样例1
输入
5
1
3 7 5 3 6
0
2

输出
11

说明
总共 5 个街区，坐地铁每次只能通过 1 个街区，坐地铁消耗时间为 0，最多可以坐 2 次地铁。
最少消耗的方案为：坐地铁通过第 2 个和第 5 个街区，其余街区步行，
最终消耗时间为 3+0+5+3+0=11。

样例2
输入
5
2
1 2 1 2 2
3
2

输出
8

说明
全程走路，不坐地铁，最终消耗时间为 1+2+1+2+2=8。

样例3
输入
10
2
4 1 12 1 6 7 2 4 4 4
3
2

输出
29

说明
坐地铁两次，分别穿越第 3 个街区以及第 5-6 个街区，
最终消耗时间为 4+1+3(地铁)+1+3×2(地铁)+2+4+4+4=29。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int MinimalConsumption(int k, int b, int m, vector<int> community) {
    vector<vector<int>> dp(community.size() + 1, vector<int>(m + 1, INT_MAX));
    dp[0][0] = 0;
    
    for (int i = 0; i < community.size(); ++i) {
      for (int j = 0; j <= m; ++j) {
        dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + community[i]);

        if (j < m) {
          for (int x = 1; x <= k && i + x <= community.size(); x++) {
            dp[i + x][j + 1] = min(dp[i + x][j + 1], dp[i][j] + x * b);
          }
        }
      }
    }
    int res = INT_MAX;
    for (int i = 0; i <= k; ++i) {
      res = min(res, dp[community.size()][i]);
    }
    return res;
  }
};

int main() {
  int n, k;
  cin >> n >> k;
  vector<int> community(n);
  for (int i = 0; i < n; ++i) {
    cin >> community[i];
  }
  int b, m;
  cin >> b >> m;
  Solution sol;
  cout << sol.MinimalConsumption(k, b, m, community);
}