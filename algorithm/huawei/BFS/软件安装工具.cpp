/*
题目内容

有一个比较复杂的软件系统需要部署到客户提供的服务器上。该软件系统的安装过程非常繁琐，为了降低操作成本，需要开发一个工具实现自动化部署。

软件的安装过程可以分成若干个小步骤，某些步骤间存在依赖关系，被依赖的步骤必须先执行完，才能执行后续的安装步骤。满足依赖条件的多个步骤可以并行执行。

请你开发一个调度程序，以最短的时间完成软件的部署。

输入描述

第一行：总步骤数 N（0<N<=10000）

第二行：N 个以空格分隔的整数，代表每个步骤所需的时间。该行所有整数之和不大于 int32。

第三行开始的 N 行：表示每个步骤所依赖的其它步骤的编号（编号从 1 开始，行号减 2 表示步骤的编号），
如果依赖多个步骤，用空格分隔。-1 表示无依赖。

测试用例确保各个安装步骤不会出现循环依赖。

输出描述

1 个数字，代表最短执行时间。

样例1
输入
4
6 2 1 2
-1
-1
1
3

输出
9

说明
一共 4 个步骤。
每个步骤所需的时间分别为 6 2 1 2。
步骤 1 和步骤 2 无依赖，可并发执行；步骤 3 依赖步骤 1；步骤 4 依赖步骤 3。
总的最小执行时间为 6+1+2=9。

样例2
输入
4
1 2 3 4
2 3
3
-1
1

输出
10

说明
步骤 1 依赖步骤 2 和 3，步骤 2 依赖步骤 3，步骤 3 无依赖，步骤 4 依赖步骤 1。
执行顺序为 3-->2-->1-->4，最小执行时间为 3+2+1+4=10。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  int n;
  cin >> n;
  vector<int> cost(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> cost[i];
  }
  cin.ignore();
  vector<vector<int>> graph(n + 1);
  vector<int> degree(n + 1, 0);
  queue<int> q;
  vector<int> finish(n + 1, 0);
  for (int i = 1; i <= n; ++i) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    int rely;
    while (ss >> rely) {
      if (rely == -1) {
        q.push(i);
        finish[i] = cost[i];
        break;
      }
      graph[rely].emplace_back(i);
      degree[i]++;
    }
  }

  /*for (int i = 1; i < graph.size(); ++i) {
    for (int j = 0; j < graph[i].size(); ++j) {
      cout << graph[i][j] << " ";
    }
    cout << endl;
  }

  auto q_back = q;
  while (!q.empty()) {
    cout << q.front() << " ";
    q.pop();
  }*/

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto v : graph[u]) {
      finish[v] = max(finish[v], finish[u] + cost[v]);
      degree[v]--;
      if (degree[v] == 0) q.push(v);
    }
    
  }
  auto it = max_element(finish.begin(), finish.end());
  cout << *it << endl;
}
