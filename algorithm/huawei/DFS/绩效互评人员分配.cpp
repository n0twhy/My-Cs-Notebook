/*
题目：绩效互评人员分配（图二分分组）

题目描述
公司组织绩效互评，为了避免有同学或者同团队的人互相打高分，需要将员工分成两组分别打分。
给定一个整数 n 和一个数组 GoodRelationships[][]，其中：

- n 表示人数。
- GoodRelationships[][] 是一个邻接表。
- GoodRelationships[i] 的元素 [a, b, c] 表示员工 i 与员工 a、b、c 之间存在“同学/同团队关系”（即 i 与这些节点相连）。

请将这 n 个人分成两组，使得：
- 在任意一组内部，不存在同学或同团队的人（即同组内不允许有边相连）。

说明
- 图是无向图：如果 j 在 GoodRelationships[i] 中，那么 i 也在 GoodRelationships[j] 中。
- 保证给出的图是联通图。
- GoodRelationships[i] 不包含 i，也不会包含重复值。
- n 满足 1 <= n <= 100（题面给出的区间为 [1,100]）。
- 每个 GoodRelationships[i] 的元素范围为 [0, GoodRelationships.length - 1]。
（若你的题库给出的范围不完全一致，以题面/输入样例为准。）

输入描述
输入以“数组形式的员工关系邻接表”给出：
- 第一行数字：n，表示有 N 个顶点（员工编号从 0 开始）。
- 接下来一共 N 行：
  第 i 行代表第 i 个顶点与其有关系的同学/团队成员（即 i 与哪些节点相邻）。
  （每行是该节点的邻接点编号列表，使用空格分隔。）

输出描述
- 分组方案按照节点编号从小到大排序输出。
- 如两个方案都满足要求，则选择“字典序更靠前”的方案输出：
  比较时先比较分组1的第一个节点编号；若相同，比较分组1的第二个节点……以此类推；
  若分组1完全相同，则比较分组2。
- 若无法将图分成符合条件的两组，则输出 -1。

样例 1
输入：
4
1 3
0 2
1 3
0 2
输出：
0 2
1 3

说明：
无向图如下（图在题面中给出）。

样例 2
输入：
4
1 2 3
0 2
0 1 3
0 2
输出：
-1

说明：
无向图如下（图在题面中给出）。
*/

#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  cin >> n;
  
  vector<vector<int>> graph(n);
  vector<int> color(n, -1);
  vector<vector<int>> res(2);
  cin.ignore();
  for (int i = 0; i < n; ++i) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    int tmp;
    while (ss >> tmp) {
      graph[i].emplace_back(tmp);
    }
  }

  color[0] = 0;
  queue<int> q;
  q.push(0);
  res[color[0]].emplace_back(0);
  while (!q.empty()) {
    auto u = q.front();
    q.pop();

    for (auto v : graph[u]) {
      if (color[v] == -1) {
        color[v] = 1 - color[u];
        q.push(v);
        res[color[v]].emplace_back(v);
      }
      if (color[v] == color[u]) {
        cout << -1 << endl;
        return 0;
      }
    }
  }

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < res[i].size(); ++j) {
      cout << res[i][j] << " ";
    }
    cout << "\n";
  }

}



/*
#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<int>> AssignGroups(vector<vector<int>>& employee) {
    int n = employee.size();
    vector<int> colors(n, -1);
    queue<int> q;
    
    q.push(0);
    colors[0] = 0;

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (auto v : employee[u]) {
        if (colors[v] == -1) {
          colors[v] = 1 - colors[u];
          q.push(v);
        } else if (colors[v] == colors[u]) {
          return {{-1}};
        }
      }
    }

    vector<vector<int>> res(2);
    for (int i = 0; i < n; ++i) {
        res[colors[i]].push_back(i);
    }
    // 编号天然递增，无需再排序

    // 字典序最小：让 res[0] 是字典序较小的那个
    if (res[1] < res[0]) swap(res[0], res[1]);

    return res;
  }
};

int main() {
  int n;
  cin >> n;
  cin.ignore();
  vector<vector<int>> employee(n);
  for (int i = 0; i < n; ++i) {
      string line;
      getline(cin, line);
      stringstream ss(line);
      int x;
      while (ss >> x) {
          employee[i].push_back(x);
      }
  }

  Solution sol;
  auto res = sol.AssignGroups(employee);

  if (res.size() == 1 && res[0].size() == 1 && res[0][0] == -1) {
      cout << -1 << endl;
      return 0;
  }

  for (auto& group : res) {
      for (int i = 0; i < (int)group.size(); ++i) {
          cout << group[i];
          if (i + 1 < (int)group.size()) cout << " ";
      }
      cout << endl;
  }
  return 0;
}
*/