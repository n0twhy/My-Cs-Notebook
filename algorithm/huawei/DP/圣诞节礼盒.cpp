/*
题目：圣诞节礼盒

题目内容
圣诞节到了，小明的妈妈准备了很多圣诞礼盒，礼盒大小不同。
小明在玩堆盒子的游戏，妈妈问小明怎样堆盒子才能使堆出的高度最高。

每个礼盒的大小由长、宽、高表示。
堆盒子时要求：下面的盒子长、宽、高都必须严格大于上面的盒子（不包含等于）。

请你帮助小明堆出最高的一堆礼盒。
总高度定义为所选礼盒高度之和。

输入描述
第一行输入礼盒个数 N。
接下来输入 N 行，每行包含 3 个整数，表示每个礼盒的长、宽、高。

参数范围：
礼盒数量不超过 1000。
每个盒子的长、宽、高取值范围为 1~10。

输出描述
输出一行，表示能堆出的盒子最高高度。

样例1
输入
4
1 1 1
2 3 4
3 6 7
4 5 6

输出
12

说明
选择 1、2、3 号盒子，3 个盒子堆出的高度最高，
总高度为 1+4+7=12。

样例2
输入
4
1 1 1
1 1 1
2 2 2
2 2 2

输出
3

说明
一种可行选择是 1 号和 3 号盒子，堆出的最高高度为 1+2=3。
*/

#include <bits/stdc++.h>
#include <climits>
#include <vector>
using namespace std;

class Solution {
public:
  int ChristmasBox(vector<vector<int>> &boxs) {
    sort(boxs.begin(), boxs.end());

    vector<int> dp(boxs.size());
    int res = INT_MIN;
    for (int i = 0; i < boxs.size(); ++i) {
      int curr_height = boxs[i][2];
      for (int j = 0; j < i; ++j) {
        if (boxs[i][0] > boxs[j][0] && boxs[i][1] > boxs[j][1] && boxs[i][2] > boxs[j][2]) {
          curr_height = max(curr_height, dp[j] + boxs[i][2]);
        }
      }
      dp[i] = max(dp[i], curr_height);
      res = max(res, dp[i]);
    }
    
    return res;
  }
};

int main() {
  int n;
  cin >> n;
  vector<vector<int>> boxs(n, vector<int>(3));

  for (int i = 0; i < n; ++i) {
    cin >> boxs[i][0] >> boxs[i][1] >> boxs[i][2];
  }

  Solution sol;
  cout << sol.ChristmasBox(boxs) << endl;
}