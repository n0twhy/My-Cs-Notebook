/*
力扣 739. 每日温度
https://leetcode.cn/problems/daily-temperatures/

题目描述：
给定一个整数数组 temperatures，表示每天的温度，返回一个数组 answer，其中 answer[i] 是指：对于第 i 天，下一个更高温度出现在几天后。

如果在这之后气温都不会再升高，请在该位置用 0 代替。

输入描述：
输入一行，若干整数从左到右依次为 temperatures[0], temperatures[1], ...（空格分隔）。

输出描述：
输出一行，若干整数依次为 answer[0], answer[1], ...（空格分隔）。

样例 1：
输入：
73 74 75 71 69 72 76 73
输出：
1 1 4 2 1 1 0 0

样例 2：
输入：
30 40 50 60
输出：
1 1 1 0

样例 3：
输入：
30 60 90
输出：
1 1 0

提示：
- 1 <= temperatures.length <= 10^5
- 30 <= temperatures[i] <= 100
*/
#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
  vector<int> temperature(vector<int> &nums) {
    stack<pair<int, int>> st;
    st.push({nums[0], 0});
    vector<int> ans(nums.size());
    for (int i = 1; i < nums.size(); ++i) {
      while (!st.empty() && nums[i] > st.top().first) {
        auto [x, y] = st.top();
        st.pop();
        ans[y] = i - y;
      }
      if (st.empty() || nums[i] <= st.top().first) {
        st.push({nums[i], i});
      }
    }
    while (!st.empty()) {
      auto [x, y] = st.top();
      st.pop();
      ans[y] = 0;
    }
    return ans;
  }
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  int n;
  vector<int> nums;
  while (ss >> n) {
    nums.emplace_back(n);
  }
  Solution sol;
  vector<int> ans = sol.temperature(nums);
  for (int i = 0; i < ans.size(); ++i) {
    cout << ans[i] << " ";
  }
}