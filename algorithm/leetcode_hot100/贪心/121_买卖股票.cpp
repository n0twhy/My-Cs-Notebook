/*
力扣 121. 买卖股票的最佳时机 I
https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/

题目描述：
给定一个数组 prices，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。

你只能选择某一天买入这只股票，并选择在未来的某一个不同的日子卖出该股票。设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0。

输入描述：
输入一行，若干整数从左到右依次为 prices[0], prices[1], ...，表示每天的股票价格（空格分隔）。

输出描述：
输出一行一个整数，表示最大利润。

样例 1：
输入：
7 1 5 3 6 4
输出：
5
说明：在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6 - 1 = 5。
注意利润不能是 7 - 1 = 6，因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。
（题面中天数从 1 开始计数，对应数组下标为 1 与 4。）

样例 2：
输入：
7 6 4 3 1
输出：
0
说明：在这种情况下没有交易完成，所以最大利润为 0。

提示：
- 1 <= prices.length <= 10^5
- 0 <= prices[i] <= 10^4
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int maxprofit(vector<int> &nums) {
    int minprice = nums[0];
    int maxprof = 0;

    for (auto &i : nums) {
      minprice = min(minprice, i);
      maxprof = max(i - minprice, maxprof);
    }
    return maxprof;
  }
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  vector<int> nums;
  int n;
  while (ss >> n) {
    nums.emplace_back(n);
  }
  Solution sol;
  cout << sol.maxprofit(nums) << endl;
}


