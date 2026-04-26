/*
题目：无线网络（5G 频段组合质量系数）

题目背景
在 5G 网络规划中，运营商需要评估不同频段组合的资源利用率。每个频段有一个特定的带宽质量评分（数值越大表示质量越好）。
当多个连续频段被组合使用时，它们的整体利用率由「组合质量系数」决定，该系数等于组合中最差频段的质量评分乘以组合的频段数量。

作为网络优化工程师，你需要计算所有可能的连续频段组合的利用率，找出其中最大的「组合质量系数」，从而确定最优的频段分配方案。

问题定义
给定一个整数数组 nums，表示一系列连续频段的质量评分。计算所有可能的连续非空频段组合的「组合质量系数」，并返回其中的最大值。

连续非空频段组合：指一组连续的频段。
例如频段质量序列 [1,2,3] 的组合包括：
- [1]、[2]、[3]
- [1,2]、[2,3]
- [1,2,3]

组合质量系数：该组合中最低质量评分乘以频段数量。

输入描述
第 1 行：整数 n，表示频段数量（1 <= n <= 10000）。

第 2 ~ n+1 行：n 个整数，表示每个频段的质量评分（1 <= nums[i] <= 10000）。

输出描述
一个整数，表示所有组合中最大的「组合质量系数」。

样例1
输入：
2
1
2

输出：
2

说明：
组合 [1] -> 系数 = 1 x 1 = 1
组合 [2] -> 系数 = 2 x 1 = 2
组合 [1,2] -> 最低质量 = 1，频段数 = 2 -> 系数 = 1 x 2 = 2
最大系数为 2，来自 [2] 或 [1,2]。

样例2
输入：
3
5
3
4

输出：
9

说明：
组合 [5] -> 最低质量 = 5，频段数 = 1 -> 系数 = 5 x 1 = 5
组合 [5,3] -> 最低质量 = 3，频段数 = 2 -> 系数 = 3 x 2 = 6
组合 [5,3,4] -> 最低质量 = 3，频段数 = 3 -> 系数 = 3 x 3 = 9
组合 [3] -> 最低质量 = 3，频段数 = 1 -> 系数 = 3 x 1 = 3
组合 [3,4] -> 最低质量 = 3，频段数 = 2 -> 系数 = 3 x 2 = 6
组合 [4] -> 最低质量 = 4，频段数 = 1 -> 系数 = 4 x 1 = 4
最大系数为 9，来自组合 [5,3,4]。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int Wifi(vector<int> &nums) {
    stack<int> st;
    int n = nums.size();
    vector<int> L(n, -1);
    vector<int> R(n, n);

    st.push(0);
    for (int i = 1; i < n; ++i) {
      while (!st.empty() && nums[st.top()] > nums[i]) {
        int pos = st.top();
        st.pop();
        R[pos] = i;
        if (!st.empty()) L[pos] = st.top();
      }
  
      st.push(i);
    }

    while (!st.empty()) {
      int pos = st.top();
      st.pop();
      if (!st.empty()) L[pos] = st.top();
    }


    int ans = 0;
    for (int i = 0; i < n; ++i) {
      int length = R[i] - L[i] - 1;
      ans = max(ans, nums[i] * length);
    }

    return ans;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> nums(n);
  for (int i = 0; i < n; ++i) {
    cin >> nums[i];
  }
  Solution sol;
  cout << sol.Wifi(nums) << endl;
}