/*
题目内容
给你一个整数 N（1 < N <= 256）。
它的一个分解可以表示为：
N = a1 * a2 * a3 * ... * ax
其中 1 < ai <= aj（当 i <= j 时），即分解因子按非递减顺序排列。

对于整数 N，请按字典序依次输出它的每一个分解结果。

例如，给定整数 24，输出是：
24=2*2*2*3
24=2*2*6
24=2*3*4
24=2*12
24=3*8
24=4*6
24=24

解答要求
时间限制：C/C++ 1000ms，其他语言 2000ms
内存限制：C/C++ 256MB，其他语言 512MB

输入描述
输入只有一个整数 N。

输出描述
按照字典序，依次输出整数 N 的每一个分解。

样例1
输入
11

输出
11=11

样例2
输入
12

输出
12=2*2*3
12=2*6
12=3*4
12=12
*/

#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
  vector<string> disolve(int num) {
    vector<string> ans;
    vector<int> curr_nums;
    vector<vector<int>> int_ans;
    function<void(int, int)> dfs = [&](int remaining, int start){
      if (remaining == 1) {
        int_ans.emplace_back(curr_nums);
        return;
      } 

      for (int i = start; i <= remaining; ++i) {
        if (remaining % i) continue;
        auto backup = curr_nums;
        curr_nums.emplace_back(i);
        dfs(remaining / i, i);
        curr_nums = backup;
      }
    };
    dfs(num, 2);
    for (auto v : int_ans) {
      string tmp = to_string(num) + "=";
      for (auto i : v) {
        tmp += to_string(i) + "*";
      }
      tmp.pop_back();
      ans.emplace_back(tmp);
    }
    return ans;
  }
};

int main() {
  int num;
  cin >> num;
  Solution sol;
  vector<string> ans = sol.disolve(num);
  for (int i = 0; i < ans.size(); ++i) {
    cout << ans[i] << endl;
  }
}