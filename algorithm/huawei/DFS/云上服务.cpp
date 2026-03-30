/*
题目：云上服务

小明将云服务看做一棵树。每个云服务在发布前尚未解决的问题称为该云服务的遗留问题（遗留问题包含以该云服务为根节点的树上所有节点的问题）。
DI 值（遗留问题缺陷密度）作为评估发布风险的指标，当云服务 DI 值小于等于阈值时才准许发布，否则需要整改后重新评估。
现有一批云服务树，已给出每个节点的问题数量，请计算风险云服务的个数。

计算公式：
DI 值 = 5 * 严重问题数 + 2 * 一般问题数
其中：
- 严重问题数：该云服务根节点子树中，所有“严重问题”(C=0) 的问题数量求和
- 一般问题数：该云服务根节点子树中，所有“一般问题”(C=1) 的问题数量求和

输入描述
第一行输入：
M 和 N（M ≤ 100000, N ≤ 1000），使用空格分隔。
- M：云服务阈值
- N：接下来有 N 行问题统计数据

接下来输入 N 行，每行 4 个字段：
A_i  B_i  C_i  D_i
- A_i：服务节点编号（字符串），长度 1~5，由小写英文字母或 '*' 组成
- B_i：A_i 的父节点（字符串），当 A_i 为云服务根节点时无父节点，B_i 用 '*' 表示
- C_i：问题级别（取值 {0,1}）
  - 0 表示严重问题
  - 1 表示一般问题
- D_i：该节点该级别的问题数量（D_i ≤ 1000）

说明：
- 输入保证只出现树的关系，不会出现连通图。
- 输入描述中“只出现树关系”意味着整套数据由若干棵树构成；B_i='*' 的节点即为各树的根（云服务节点）。

输出描述
输出一个整数，表示风险云服务个数。
当某棵云服务树的 DI 值 > M 时，该云服务为风险云服务。

样例一
输入：
40 12
a * 0 2
a * 1 2
b a 0 3
b a 1 5
c a 1 3
d a 0 1
d a 1 3
e b 0 2
f * 0 8
f * 1 10
g f 1 2
h * 0 4

输出：
2

解释：
该样例的云服务根为 a、f、h。
逐棵计算子树内严重/一般问题数量并带入 DI 公式，最终有 2 棵 DI>40 的云服务为风险云服务。

样例二
输入：
50 10
b a 1 5
a * 0 2
b a 0 3
c a 1 3
d a 0 1
a * 1 2
d a 1 3
e b 0 2
f b 1 1
g c 1 2

输出：
1
*/

#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
using namespace std;

class Solution {
public:
  int dfs(string node) {
    int tmp_di = di[node];
    for (auto child : graph[node]) {
      tmp_di += dfs(child);    
    }
    return tmp_di;
  }

  int CloudServer(vector<string> &cloud_servers, int limit) {
    unordered_set<string> servers;
    for (auto &s : cloud_servers) {
      stringstream ss(s);
      string node, pertain;
      int type, prob;
      ss >> node >> pertain >> type >> prob;
      int tmp_di = type == 0? 5 * prob : 2 * prob;
      
      if (pertain != "*") graph[pertain].push_back(node);
      else if (!servers.count(node)) servers.insert(node);
      di[node] += tmp_di;
    }

    int error = 0;

    for (auto &c : servers) {
      if (dfs(c) > limit) error++;
    }

    return error;
  }
private:
  unordered_map<string, vector<string>> graph;
  unordered_map<string, int> di;
};

int main() {
  int limit, n;
  cin >> limit >> n;
  vector<string> input;
  cin.ignore();
  for (int i = 0; i < n; ++i) {
    string tmp;
    getline(cin, tmp);
    input.push_back(tmp);
  }
  Solution sol;
  cout << sol.CloudServer(input, limit) << endl;
}