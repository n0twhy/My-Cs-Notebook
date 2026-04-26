/*
致歉
注意本题的相对顺序是无法改变的，详情请见小明补充的样例4。

题目内容

某媒体处理服务负责接收来自多个媒体发送源的媒体包，并根据收到的媒体包进行媒体渲染处理。当前有这样一个需求：给定收到的媒体包序列号列表，计算发送该媒体包的最少发送源个数。

约束：

1. 任意媒体包序列号 seqs[i] 满足：
   0<=seqs[i]<=65535

2. 网络上没有重传媒体包，即：同一个发送源发送的媒体包序列号不会重复，且序列号每次加 1（不考虑回绕问题，65535 是发送源发送的最后一个媒体包序列号）；
   如果收到的媒体包序列号不满足该规则，说明这些媒体包必然来自于多个发送源。

3. 1<=seqs.length()<=10^5

输入描述

第一行：
seqs 列表长度 n

第二行：
seqs 列表元素，元素之间通过空格隔开

输出描述

最少媒体包发送源个数

样例1
输入
11
1 2 3 4 5 6 7 8 9 10 10

输出
2

说明
媒体包发送源1：
1 2 3 4 5 6 7 8 9 10

媒体包发送源2：
10

媒体发送源个数为 2，因此输出 2。

样例2
输入
5
65535 0 1 2 3

输出
2

说明
媒体包发送源1：
65535

媒体包发送源2：
0 1 2 3

媒体发送源个数为 2，因此输出 2。

样例3
输入
18
1 2 2 3 3 4 4 5 5 6 6 7 7 8 8 9 9 10

输出
2

说明
媒体包发送源1：
1 2 3 4 5 6 7 8 9 10

媒体包发送源2：
2 3 4 5 6 7 8 9

媒体发送源个数为 2，因此输出 2。

样例4
输入
8
2 2 2 2 1 1 1 1

输出
8
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int resources(vector<int> &packets) {
    unordered_map<int, int> need;
    int res = 0;
    for (int i = 0; i < packets.size(); ++i) {
      if (need[packets[i]] >= 1) {
        need[packets[i]]--;
      } else {
        res++;
      }
      need[packets[i] + 1]++;
    }
    return res;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> packets(n);
  for (int i = 0; i < n; ++i) {
    cin >> packets[i];
  }
  Solution sol;
  cout << sol.resources(packets) << endl;
}