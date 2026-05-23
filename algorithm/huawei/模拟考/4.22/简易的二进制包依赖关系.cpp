/*
题目内容
一个项目中，除了自研的代码外，还会依赖很多二进制包（后续简称为包），这些包也会依赖其它的包，每个被依赖的包还有版本号的要求。本题借鉴了包管理的思想，需要完成一个简易的包依赖关系分析和处理的模型，要求对输入的一组依赖关系进行分析，判断是否存在循环依赖，如果有循环依赖则输出不合理；否则进一步对依赖包的版本号进行规整，并输出规整后的依赖关系串。

该模型的输入是一组依赖关系，其数据定义如下：

以图为例，输入的数据为：
依赖关系 1：{1,3,11}，表示包 1 依赖包 3 的 11 版本；
依赖关系 2：{2,3,12}，表示包 2 依赖包 3 的 12 版本。

因此，该依赖关系的数据结构由三个属性组成：
序号：任意正整数，用于唯一标识一个二进制包（如上图中的包1）。
依赖包序号：任意正整数，表示该包所依赖的另一个包的序号（如上图中的包3）。
依赖包版本号：正整数（如上图中的包3的11版本）。而且 1 <= 版本号 <= 99。

基于该依赖关系的数据分析，我们需要进行如下的判断和处理：

1、判断包依赖关系中是否存在循环依赖
包之间的依赖关系不能形成循环，例如，包 1 依赖包 2，包 2 依赖包 3，包 3 又依赖包 1，这种情况属于循环依赖，不合理。
注：版本号不纳入循环依赖与否的判断

2、对包依赖关系的版本号进行规整处理
如果判断包依赖关系是合理的，我们需要进一步对一对依赖包的版本号进行规整处理。对于多个包依赖同一个包的情况，我们需要判断被依赖包的不同版本号，取其中的最大值，然后输出新的依赖关系（具体见下面的样例描述）。

输入描述
每次会输入两组依赖关系的信息，分别解析和输出两组结果，每组的格式定义如下：

包依赖关系的个数：正整数 n（0 < n < 100），表示待输入的包依赖关系的个数。

包依赖关系：每行表示一个依赖关系，格式为：序号，依赖包序号，依赖包版本号，共 n 行。

序号，依赖包序号，依赖包版本号的定义参考前文的数据定义章节。

注：这两组依赖关系是完全独立的，建议从方法复用角度，合理对方法进行封装，避免出现重复代码。

输出描述
基于输入的两组数据进行处理，按顺序依次输出结果即可，每组的输出要求如下：

1、判断输入的二进制包数组是否合理，参考「1、判断包依赖关系中是否存在循环依赖」。如果判断是循环依赖，则输出 false；否则进行第 2 步。

2、对包依赖关系进行版本号的规整处理，参考「2、对包依赖关系的版本号进行规整处理」。处理完成后输出新的依赖关系（参考下方样例说明）。

样例1
输入
3
1,2,23
2,3,34
4,2,25
3
1,2,23
2,3,34
3,1,12

输出
1,2,25
2,3,34
4,2,25
false

说明
输入：
第一组输入的解释: 3 // 表示待输入的依赖的个数
1,2,23 // 包 1 依赖包 2 版本 23
2,3,34 // 包 2 依赖包 3 版本 34
4,2,25 // 包 4 依赖包 2 版本 25

包 1 依赖包 2 版本 23，包 2 依赖包 3 版本 34，包 4 依赖包 2 版本 25。其中不存在循环依赖；同时包 1 和包 4 共同依赖包 2，对依赖的版本号进行规整后为 25。因此整个依赖关系数组是合理的，规整完版本号后再进行输出。

第二组输入的解释: 包 1 依赖包 2，包 2 依赖包 3，包 3 依赖包 1，存在循环依赖。因此整个依赖关系数组不合理，输出 false。

输出:
第一组输出结果:
1,2,25 // 被依赖的包 2 的版本号取最大，此处原来版本号 23 需要被替换成 25
2,3,34
4,2,25

第二组输出结果:
false

提示
1、自己依赖自己也属于循环依赖。
2、输入的依赖关系中，包 X 依赖包 Y 只会出现一次，不需要出现多次对应使用哪个版本号的问题。
3、每个包可以依赖多个包，包 X 也可以被多个包依赖。
*/
#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
  vector<string> Reliance(vector<vector<int>> &list) {
    int n = list.size();
    unordered_map<int, pair<int, vector<int>>> graph;
    unordered_map<int, int> degree;
    unordered_map<int, int> editions;
    for (int i = 0; i < n; ++i) {
      int v, u, edition;
      v = list[i][0];
      u = list[i][1];
      edition = list[i][2];
      degree[v]++;
      if (!degree.count(u)) degree[u] = 0;
      graph[u].first = i;
      graph[u].second.emplace_back(v);
      if (edition > editions[u]) editions[u] = edition;
    }
    queue<int> q;
    int cnt = 0;
    for (auto [u, degree_cnt] : degree) {
      if (degree_cnt == 0) q.push(u);
    }
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      cnt++;

      for (auto v : graph[u].second) {
        degree[v]--;
        if (degree[v] == 0) {
          q.push(v);
        }
      }
    }
    if (cnt != degree.size()) return {"false"};
    vector<string> ans;
    for (auto vt : list) {

      int v, u, edition;
      v = vt[0];
      u = vt[1];
      edition = editions[u];
      string tmp = to_string(v) + "," + to_string(u) + "," + to_string(edition);
      ans.emplace_back(tmp);
    }
    return ans;
  }
};

int main() {
  Solution sol;
  for (int i = 0; i < 2; ++i) {
    int n; cin >> n;
    cin.ignore();
    vector<vector<int>> list(n, vector<int>(3));
    for (int j = 0; j < n; ++j) {
      string rely;
      getline(cin, rely);
      stringstream ss(rely);
      string v, u, edition;
      getline(ss, v, ',');
      getline(ss, u, ',');
      getline(ss, edition);
      list[j][0] = stoi(v);
      list[j][1] = stoi(u);
      list[j][2] = stoi(edition);
    }
    auto res = sol.Reliance(list);
    if (res[0] == "false") {
      cout << "false" << endl;
      continue;
    }
    for (int i = 0; i < res.size(); ++i) {
      cout << res[i] << endl;
    }
  }
}