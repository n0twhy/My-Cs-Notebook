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
#include <unordered_map>
using namespace std;

class Solution {
public:
  vector<vector<int>> AssignGroups(vector<vector<int>> employee) {
    unordered_map<int, int> group;
    queue<int> q;
    map<int, vector<int>> graph;

    for (auto &v : employee) {
      for (int i = 1; i < v.size(); ++i) {
        graph[v[0]].emplace_back(v[i]);
        cout << v[0] << " man: " << v[i] << " ";
      }
      cout << endl;
    }

    q.push(0);
    group[0] = 0;
    int curr_group = 0;
    unordered_map<int, vector<int>> res;
    res[0].emplace_back(0);
    while (!q.empty()) {
      curr_group = (curr_group + 1) % 2;
      cout << "current group: " << curr_group << endl;
      int size = q.size();
      for (int i = 0; i < size; ++i) {
        int curr_man = q.front();
        cout << "出队： " << curr_man << endl;
        q.pop();
        for (auto &neighbor : graph[curr_man]) {
          cout << "neighbor: " << neighbor << endl;
          if (!group.count(neighbor)) {
            group[neighbor] = curr_group;
            res[curr_group].emplace_back(neighbor);
            cout << "gourp: " << curr_group << " curr man: " << curr_man << " neighbor: " << neighbor << endl;
            q.push(neighbor);
          } else if (group[neighbor] != curr_group) {
            return {{-1}};
          }
        }
      }
    }
    
    vector<vector<int>> ans;
    for (auto &it : res) {
      ans.push_back(it.second);
    }
    return ans;
  }
};

int main() {
  int n;
  cin >> n;
  cin.ignore();
  vector<vector<int>> employee;
  for (int i = 0; i < n; ++i) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    int tmp;
    vector<int> tmp_v;
    while (ss >> tmp) {
      tmp_v.emplace_back(tmp);
    }
    employee.push_back(tmp_v);
  }

  Solution sol;
  vector<vector<int>> res = sol.AssignGroups(employee);

  for (int i = 0; i < res.size(); ++i) {
    for (int j = 0; j < res[i].size(); ++j) {
      cout << res[i][j] << " ";
    }
    cout << endl;
  }
}