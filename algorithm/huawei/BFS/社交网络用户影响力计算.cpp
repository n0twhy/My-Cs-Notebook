/*
题目内容

社交网络拓扑图中的节点表示社交网络中的用户，边表示两个用户之间的社交连接，边是无向的，两个用户最多只有一条直接相连的边。
用户的影响力定义为：从某个社交网络用户开始，找出所有可以在 K 跳（直接或间接关系）内接触到的其他用户的总个数。

请实现一个程序，计算给定社交网络中某个用户在 k 跳范围内的影响力。

输入描述

第一行输入 N M K（三个空格分隔的正整数）：
N 代表社交网络连接总数，
M 代表需要计算影响力的用户编号，
K 代表计算影响力的范围。

1<=N,K<=1000, 0<=M<1000

接下来的 N 行，每行两个整数 X Y（0<=X,Y<=1000），代表社交网络中一条直接连接的边，
如 "1 2" 代表 1 号与 2 号用户互相直接连接。

用例确保输入有效，无需进行校验。

输出描述

输出 M 用户在 K 跳范围内的影响力。

样例1
输入
5 0 2
0 1
1 2
2 3
3 4
4 0

输出
4

样例2
输入
8 0 3
0 1
0 2
0 3
3 4
2 5
5 4
2 3
1 5

输出
5
*/
#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  int n, m, k;
  cin >> n >> m >> k;
  
  vector<vector<int>> graph(1001);
  for (int i = 0; i < n; ++i) {
    int user_1, user_2;
    cin >> user_1 >> user_2;
    graph[user_1].emplace_back(user_2);
    graph[user_2].emplace_back(user_1);
  }
  unordered_set<int> connected;
  queue<int> q;
  int jump = 0;
  for (auto v : graph[m]) {
    q.push(v);
  }
  connected.insert(m);
  while (!q.empty()) {
    int size = q.size();
    jump++;
    if (jump > k) break;
    for (int i = 0; i < size; ++i) {
      int u = q.front();
      connected.insert(u);
      q.pop();
      for (auto v : graph[u]) {
        if (connected.count(v)) continue;
        q.push(v);
      }
    }
  }

  cout << connected.size() - 1 << endl;
  
}