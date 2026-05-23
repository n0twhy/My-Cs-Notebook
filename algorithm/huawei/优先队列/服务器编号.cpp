/*
题目描述
数据中心有 M 个服务器(编号 1−M)，现在有 N 个任务(编号 1−N)需要执行。
当一个任务开始执行时，会独占编号最小的空闲服务器，执行完后会立即释放该服务器。
任务按照启动时间的先后顺序执行（所有任务的启动时间都不相同）。
请返回编号为 K 的任务执行时占用的服务器编号。

输入描述
第一行是用空格分隔的三个整数：M, N, K
M, N, K 分别代表服务器个数、任务数、需要查询的任务编号，
1 <= K <= N <= M <= 10^4

接下来 N 行是编号 1−N 的任务信息，
每行是用空格分隔的两个整数，分别代表任务的启动时间、执行需要的时间。
1 <= 启动时间, 执行时间 <= 10^5

输出描述
输出编号为 K 的任务执行时占用的服务器编号。

样例1
输入
4 3 2
1 2
4 6
3 3

输出
2

说明
编号为 1 的任务(启动时间为 1)占用 1 号服务器；
编号为 3 的任务(启动时间为 3)执行时，编号为 1 的任务已经执行完，1 号服务器空闲，所以占用 1 号服务器；
编号为 2 的任务(启动时间为 4)执行时，编号为 3 的任务还没有执行完，所以占用 2 号服务器。

样例2
输入
2 1 1
1 5

输出
1

说明
只有 1 个任务，占用 1 号服务器。
*/

#include <bits/stdc++.h>
#include <cinttypes>
#include <functional>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int main() {
  int n, m, k;
  cin >> m >> n >> k;
  vector<pair<int, pair<int, int>>> missions(n);
  for (int i = 0; i < missions.size(); ++i) {
    missions[i].first = i + 1;
    cin >> missions[i].second.first >> missions[i].second.second;
  }

  sort(missions.begin(), missions.end(), [](const pair<int, pair<int, int>>& a, const pair<int, pair<int, int>> &b){
    return a.second.first < b.second.first;
  });

  int server_id = 1;
  priority_queue<int, vector<int>, greater<>> pq;
  // [end, mission_id]
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> occupying;
  vector<int> occupy(n + 1);
  

  for (auto [mission_id, p] : missions) {
    auto [start, process_time] = p;
    //cout << "[start, process_time]: " << start << ", " << process_time << endl;
    while (!occupying.empty() && occupying.top().first <= start) {
      int return_mission_id = occupying.top().second;
      occupying.pop();
      //cout << "has done: " << occupying.top().first << ", " << return_mission_id << endl;
      pq.push(occupy[return_mission_id]);
    }

    int end = start + process_time;
    
    occupying.push({end, mission_id});
    
    if (!pq.empty()) {
      //cout << "pq, letsgo: " << pq.top() << endl;
      int allocate = pq.top();
      pq.pop();
      occupy[mission_id] = allocate;
    } else {
      //cout << "server_id, letsgo: " << server_id << endl;
      occupy[mission_id] = server_id++;
    }
    //cout << mission_id << ": " << occupy[mission_id] << endl;
  }

  cout << occupy[k] << endl;
}