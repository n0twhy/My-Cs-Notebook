/*
题目内容
团团过年收获了很多压岁钱，妈妈帮他开了账户去投资。
现在给出 n 天内投资收益情况，选出连续多少天的收益总和最大，这个收益是多少。

输入描述
第一行是一个整数 n，表示天数，
n 的范围为 [0,1000]。

第二行是 n 个整数组成的一个数组，表示每天的收益，
有正数也有负数，范围为 [-10000,100000]。

输出描述
输出一个整数，表示选取的连续天数的最大收益。

样例1
输入
3
1 0 -1

输出
1

说明
3 天的收益，第一天最多，输出为 1。

样例2
输入
7
2 -4 3 -1 2 -4 3

输出
4

说明
7 表示 7 天的收益，选取第 3 天到第 5 天的收益最大，
3 - 1 + 2 = 4。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <climits>
using namespace std;

class Solution {
public:
  int maxprofit(vector<int> &days) {
    vector<int> dp(days.size(), INT_MIN);
    dp[0] = days[0];
    int max_profit = dp[0];
    for (int i = 1; i < days.size(); ++i) {
      dp[i] = max(days[i], days[i] + dp[i - 1]);
      max_profit = max(dp[i], max_profit);
    }
    return max_profit;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> days(n);
  for (int i = 0; i < n; ++i) {
    cin >> days[i];
  }
  Solution sol;
  cout << sol.maxprofit(days) << endl;
}