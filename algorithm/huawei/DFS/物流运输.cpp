/*
题目内容
物流公司每天都要处理很多物流的运输工作，整个城市共有 N 个地点。共有 N−1 条公路，每 2 个地点之间都能通过公路连通。物流公司总部位于 1 号地点。

今天有一辆物流运输车共有 M 条物流运输任务，物流运输车每天的工作流程如下：

先要从总部出发去收取所有的寄件货物，收到所有货物后回到总部扫描货物，再从总部出发将货物送至所有的送件地址，送完后最终回到总部，算作完成了今天的运输工作。

请问该辆物流运输车今天最少行驶多少路程可以完成今天的运输工作，运输任务不分先后。

输入描述
对于每组数据，第一行有 2 个整数，依次为 N、M，表示有 N 个地点和 M 条物流任务，数字用空格分开。

约束：3 ≤ N ≤ 10^5，1 ≤ M ≤ 10^5。

接下来有 N−1 行，每行有 3 个整数，依次为 u、v、c，表示从 u 到 v 有一条公路，公路里程为 c。

约束：1 ≤ u, v ≤ N，1 ≤ c ≤ 10^5。输入保证所有地点连通。

接下来有 M 行，每行有 2 个整数，依次为 s、t，表示寄件任务从 s 寄到 t。

约束：2 ≤ s, t ≤ N，s ≠ t。

输出描述
输出一个整数，表示该辆物流运输车最少行驶多少路程能够完成今天的运输工作。

样例1
输入
4 2
2 1 1
1 3 2
4 3 2
3 2
4 2
输出
10
说明
运输车从地点 1 开到地点 3 接收任务 1 物品，再开到地点 4 接收任务 2 物品，回到总部 1 扫描，扫描后将任务 1 和任务 2 的物品送到地点 2，最终回到总部 1，总共行驶里程 10。

样例2
输入
5 2
2 1 1
1 3 2
4 3 2
1 5 3
4 2
5 4
输出
24
说明
运输车从地点 1 开到地点 5 接收任务 2 物品，再开到地点 4 接收任务 1 物品，回到总部 1 扫描，扫描后开到地点 4 完成任务 2，再开到地点 2 完成任务 1，最终回到总部 1，总共行驶里程 24。
*/

#include <bits/stdc++.h>
#include <functional>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  int MinimalPath(int N, vector<vector<int>> &paths, vector<pair<int, int>> &missions) {
    vector<vector<pair<int, int>>> graph(N + 1);
    for (auto v : paths) {
      int place_1 = v[0];
      int place_2 = v[1];
      int dist = v[2];
      graph[place_1].emplace_back(place_2, dist);
      graph[place_2].emplace_back(place_1, dist);
    }
    vector<bool> is_pickup(N + 1, false);   // 上半场：寄件点集合
    vector<bool> is_delivery(N + 1, false); // 下半场：送件点集合
    for (auto &[u, v] : missions) {
      is_pickup[u] = true;
      is_delivery[v] = true;
    }
    int path = 0;
    function<bool(int, int, bool)> dfs = [&](int u, int p, bool is_pick) -> bool {
      bool has_found = false;
      if (is_pick) {
        has_found = is_pickup[u];
      } else {
        has_found = is_delivery[u];
      }

      
      for (auto &[v, dist] : graph[u]) {
        if (v == p) continue;
        bool has_child = dfs(v, u, is_pick);
        if (has_child) {
          has_found = true;
          path += 2 * dist;
        }
      }
      
      return has_found;
    };
    dfs(1, 0, true);
    dfs(1, 0, false);
    return path;
  }
};

int main() {
  int N, M;
  cin >> N >> M;
  vector<vector<int>> paths(N, vector<int>(3));
  for (int i = 0; i < N - 1; ++i) {
    for (int j = 0; j < 3; ++j) {
      cin >> paths[i][j];
    }
  }
  vector<pair<int, int>> missions(M);
  for (int i = 0; i < M; ++i) {
    cin >> missions[i].first >> missions[i].second;

  }
  Solution sol;
  cout << sol.MinimalPath(N, paths, missions) << endl;
  return 0;
}