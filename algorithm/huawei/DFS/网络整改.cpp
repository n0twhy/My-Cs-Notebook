/*
题目内容
在一个树形的网络拓扑中，有 n 台设备，编号 1 到 n，其中我们固定 1 为根设备：根设备下可下挂多台设备（如设备编号 2、3），以此类推每一台设备下都可能下挂 1 台或者多台设备，最后没有下挂设备的设备成为边缘设备（如设备 3、5、6、7）。

现在我们希望对网络进行整改，将组网中的部分设备移除，使得所有的边缘设备到根设备的距离相同，请你计算最少需要移除多少台设备。

如题目示例图：只需要移除 3 号和 5 号设备，可以使剩下的所有边缘设备（6、7）到根设备的距离相同。

注：整个网络是单个连通的树型组网且没有环。

输入描述
用例第一行为一个整数 n（3 ≤ n ≤ 5000），代表网络设备数目。

接下来 n−1 行每行包含两个整数 u、v（1 ≤ u, v ≤ n，u ≠ v），代表设备 u 与设备 v 相连接（注意仅代表连接关系，不表明确父子关系）。

注：我们保证每个设备的编号都小于等于 n，且不重复；n 个网络设备，必然有 n−1 条连接。

输出描述
输出最少移除多少台设备，可以使得剩下的所有边缘设备到根设备距离都相同。

样例1
输入
7
1 2
1 3
2 4
2 5
4 6
4 7
输出
2
说明
如题目实例图：移除 3 号和 5 号 2 台设备，可以使剩下的所有边缘设备（6 / 7）到根设备的距离相同。

样例2
输入
5
4 1
2 4
5 1
5 3
输出
0
说明
该用例中的树形图可见不需要移除任何设备就满足边缘设备（2 和 3）到根设备 1 的距离都相等。

样例3
输入
7
1 2
2 3
3 4
1 5
1 6
1 7
输出
2
说明
该用例移除设备 4 后，再移除设备 3 即可。
*/

#include <bits/stdc++.h>
#include <climits>
#include <functional>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  int remove(vector<pair<int, int>> &devices) {
    vector<vector<int>> graph(devices.size() + 2);
    for (auto &[u, v] : devices) {
      graph[u].emplace_back(v);
      graph[v].emplace_back(u);
    }
    int D;
    function<int(int, int, int)> dfs = [&](int u, int p, int d) -> int {
      if (d == D) return 1;
      int nodes = 0;
      for (auto &v : graph[u]) {
        if (v == p) continue;
        nodes += dfs(v, u, d + 1);
      }
      return nodes == 0 ? nodes : nodes + 1;
    };
    int res = INT_MAX;
    int N = devices.size() + 1;
    for (D = 1; D <= devices.size() + 1; ++D) {
      int retain = dfs(1, 0, 1);
      int deleted = N - retain;
      res = min(res, deleted);
    }
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<pair<int, int>> devices(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    cin >> devices[i].first >> devices[i].second;
  }
  Solution sol;
  cout << sol.remove(devices) << endl;
}