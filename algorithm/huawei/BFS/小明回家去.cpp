/*
题目内容

星际漫游是需要通行证的，塔子收集多年，希望来一次跨越群星的路程，去到一个叫做蓝星的星球，听说那里有一个二字游戏，非常好玩。

很不幸的是，塔子只有若干星球之间的通行证，为了尽快到达心心念念的蓝星，塔子决定找你帮忙规划一下线路。这里规定星际漫游的耗时与经历的星球数成正比。

规定，所有的星球都被编号，从 1 开始。

解答要求
时间限制：C/C++ 800ms，其他语言：1600ms
内存限制：C/C++ 60MB，其他语言：120MB

输入
塔子拥有的通行证数目 n 以及塔子漫游的起点编号 m1（0<=m1<=100000）和终点编号 m2（0<=m2<=100000），
之后 n 行依次为 n 个通行证，每个通行证有两个数字，用空格隔开，分别表示该通行证允许通行的起点星球编号和终点星球编号。

注意，只能从起点星球飞往终点星球。

输出
计算塔子需要经过的星球数目，不包括起点星球，若无法到达目的地，则返回 -1。

样例1
输入
1 1 2
1 2

输出
1

解释
本例表示：塔子有 1 个通行证，刚好直达蓝星，经过蓝星，故输出 1。

样例2
输入
3 2 1
1 4
3 2
3 5

输出
-1

解释
拥有的三个通行证无法保证塔子能从编号为 2 的星球到达编号为 1 的蓝星，
塔子因玩不到二字游戏悲痛欲绝，因此结果为 -1。
*/

#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  int n, m1, m2;
  cin >> n >> m1 >> m2;
  cin.ignore();
  vector<vector<int>> planets(100000);
  for (int i = 1; i <= n; ++i) {
    int start, end;
    cin >> start >> end;
    planets[start].emplace_back(end);
  }

  queue<int> q;
  int length = 0;
  vector<bool> visited(100000, false);
  q.push(m1);
  visited[m1] = true;
  while (!q.empty()) {
    int size = q.size();
    for (int i = 0; i < size; ++i) {
      int u = q.front();
      q.pop();

      for (auto v : planets[u]) {
        if (v == m2) {
          cout << length + 1 << endl;
          return 0;
        }
        visited[v] = true;
        q.push(v);
      }
    }
    
    length++;
  }

  cout << -1 << endl;
}