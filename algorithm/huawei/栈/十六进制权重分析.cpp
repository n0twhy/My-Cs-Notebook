/*
题目：十六进制权重分析

题目描述
设计一个程序来处理特定的数组分析问题。

给定一个非负整数数组 arr，其中每个整数用其十六进制表示中的数字之和来表示其“权重”。
权重计算基于十六进制每一位数字的值：
- 0 ~ 9 对应权重 0 ~ 9
- A:10、B:11、C:12、D:13、E:14、F:15

你的任务是找出数组中每个元素右侧第一个具有更大“权重”的元素，并返回一个新的数组，该数组包含这些元素的索引。

如果某个元素右侧不存在更大“权重”的元素，则该位置返回 -1。

输入描述
第一行：一个整数 N，表示数组 arr 的大小（0 < N < 100000）。

第二行：N 个由空格分隔的非负整数，表示数组 arr（0 <= arri <= 0xffffffff）。

输出描述
一行：N 个整数，表示每个元素右侧第一个权重更大的元素索引；如果不存在则为 -1。

样例1
输入：
3
12 3 24

输出：
-1 2 -1

说明：
十六进制表示分别为 [C,3,18]。
对应权重分别为 [12,3,1+8=9]。

对于第一个元素 12（权重 12），其右侧没有更大权重元素，返回 -1。
对于第二个元素 3（权重 3），其右侧第一个更大权重元素是 24（权重 9），索引为 2。
对于第三个元素 24（权重 9），其右侧没有更大权重元素，返回 -1。

样例2
输入：
5
15 8 23 42 7

输出：
-1 3 3 -1 -1

说明：
十六进制表示分别为 [F,8,17,2A,7]。
对应权重分别为 [15,8,8,2+10=12,7]。

对于第一个元素 15（权重 15），其右侧没有更大权重元素，返回 -1。
对于第二个元素 8（权重 8）和第三个元素 23（权重 8），右侧第一个更大权重元素是 42（权重 12），索引为 3。
对于第四个元素 42（权重 12）和第五个元素 7（权重 7），其右侧没有更大权重元素，返回 -1。
*/

#include <bits/stdc++.h>
#include <cctype>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
  string i_to_0x(int num) {
    string s;
    while (num) {
      int digit = num % 16;
      if (digit < 10) s += to_string(digit);
      else {
        s += ('A' - 10 + digit);
      }
      num /= 16; 
    }
    reverse(s.begin(), s.end());
    return s;
  }
  vector<int> sixteen(vector<int> &nums) {
    stack<int> st;
    st.push(0);
    vector<int> res(nums.size(), -1);

    vector<int> transform;
    for (auto num : nums) {
      int sum = 0;
      string curr = i_to_0x(num);
      for (auto c : curr) {
        if (isdigit(c)) sum += c - '0';
        else sum += c - 'A' + 10;
      }
      transform.emplace_back(sum);
    }

    for (int i = 1; i < nums.size(); ++i) {
      while (!st.empty() && transform[st.top()] < transform[i]) {
        int pos = st.top();
        st.pop();
        res[pos] = i;
      }
      st.push(i);
    }
    return res;
    
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
  auto res = sol.sixteen(nums);
  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
}