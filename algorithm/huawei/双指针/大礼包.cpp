/*
题目：大礼包

题目内容
某公司对新用户推出大礼包：从任意一天注册开始，连续登录 x 天，
每天可领取一定数量的金币。

金币数量与公司设计的虚拟日历相关：
- 一年有 n 个月；
- 第 i 个月有 d_i 天；
- 每年日历都相同。

在每个月内：
- 第 1 天可领 1 个金币；
- 第 2 天可领 2 个金币；
- 第 3 天可领 3 个金币；
- 以此类推，第 k 天可领 k 个金币。

请计算：新用户注册后连续登录 x 天，最多可以获取多少金币。
注意连续登录可能会跨年。

解答要求
时间限制：C/C++ 500ms，其他语言 1000ms
内存限制：C/C++ 256MB，其他语言 512MB

输入描述
第一行输入两个整数 n 和 x，分别表示一年中的月数和连续登录天数。
第二行输入 n 个整数 d1,d2,...,dn，d_i 表示第 i 个月的天数。

参数范围：
1 <= n <= 2*10^5
1 <= d_i <= 10^6
1 <= x <= d1+d2+...+dn

输出描述
输出一个整数，表示连续登录 x 天最多可获得的金币数量。

样例1
输入
3 2
1 3 1

输出
5

解释
一年中每天获取金币序列为 {1,1,2,3,1}（按月份展开）。
若在一年中的第 3 天开始登录，最多可获取 2+3=5 个金币。

样例2
输入
3 6
3 3 3

输出
12

解释
一年中每天获取金币序列为 {1,2,3,1,2,3,1,2,3}。
若在一年中的第 1 天开始登录，最多可获取 3+1+2+3+1+2=12 个金币
（可视作从某个 3 开始跨月连续取 6 天）。

样例3
输入
5 6
4 2 3 1 3

输出
15

解释
一年中每天获取金币序列为 {1,2,3,4,1,2,1,2,3,1,1,2,3}。
若在合适起点开始连续登录 6 天，最多可获取 2+3+1+2+3+4=15 个金币。
*/

#include <bits/stdc++.h>
using namespace std;

int main() {
  
}

/*
#include <bits/stdc++.h>
#include <exception>
#include <vector>
using namespace std;

class Solution {
public:
  int BigPresent(int x, vector<int> months) {
    vector<int> pre_sum;
    int sum = 0;
    int size = months.size();
    for (int i = 0; i < size; ++i) {
      months.emplace_back(months[i]);
    }

    for (auto i : months) {
      sum += i;
      pre_sum.emplace_back(sum);
    }

    int ans = 0;

    int end = months.size() / 2;
    while (end < months.size()) {
      int start = pre_sum[end] - x;
      //cout << "start : end = " << pre_sum[end] << ": " << start << endl;
      auto it = upper_bound(pre_sum.begin(), pre_sum.end(), start);
      int pos = it - pre_sum.begin();
      //cout << "curr_pos: " << pos << endl;
      int remain_days = pre_sum[pos] - start;
      //cout << "remain : " << remain_days << endl;
      int d = months[pos];
      int total_gold = d * remain_days + (remain_days - remain_days * remain_days) / 2;
      //cout << "left_gold = " << total_gold << endl;
      int left = pos + 1;
      while (left <= end) {
        //cout << "hey" << endl;
        int n = months[left];
        total_gold += (n * n + n) / 2;
        left++;
      }
      end++;
      //cout << "total_gold = " << total_gold << endl; 
      ans = max(ans, total_gold);
    }
    return ans;
  }
};

int main() {
  int month_cnt, days;
  cin >> month_cnt >> days;
  vector<int> months(month_cnt);

  for (int i = 0; i < month_cnt; ++i) {
    cin >> months[i];
  }

  Solution sol;
  cout << sol.BigPresent(days, months) << endl;
}
*/