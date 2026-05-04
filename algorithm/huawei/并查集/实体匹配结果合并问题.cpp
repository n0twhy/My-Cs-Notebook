/*
题目内容

某业务部门有多个数据来源，现在需要对多个来源的实体数据进行去重、消歧、合并。有多个实体匹配系统（假设系统的匹配结果完全正确），每个系统从不同角度进行匹配，匹配结果是相同实体列表。

这些匹配结果中往往存在交叉重复的问题，需要对所有匹配结果进行合并去重。例如系统 A 的匹配结果是 ["1", "2"]，系统 B 的匹配结果是 ["2", "3"]，那么合并后的匹配结果是 ["1", "2", "3"]。请你按照上述逻辑，编写代码实现对匹配结果的合并去重。

输入描述

第一行输入是整数 N，表示有 N 个实体匹配系统，1<=N<=10000；

接下来 N 行是每个实体匹配系统的匹配结果（相同实体列表，每行实体数量不超过 100）；实体使用数字字符串表示（字符长度不超过 10），实体之间通过空格分开。实体种类数量不超过 100000。

输出描述

输出 M 行（M<=N），每行内容是合并后的匹配结果。

注意，每行的输出结果需要按照字典顺序进行排序；合并后的匹配结果列表之间，也需要按字典顺序进行排序。

样例1
输入
5
1 2 3
4 5
11 22
33 44 55 1
3 66

输出
1 2 3 33 44 55 66
11 22
4 5

说明
匹配结果 "1 2 3"、"33 44 55 1"、"3 66"，存在重复实体 "1" 和 "3"，故可以合并，合并后按字典序为 "1 2 3 33 44 55 66"；
另外两组结果与其他组不存在重复；
合并后的匹配结果列表之间，按字典序排序后输出，即样例输出所示内容。

样例2
输入
2
1 2
2 3

输出
1 2 3

说明
有两组匹配结果，即 "1 2" 和 "2 3"，存在重复实体 "2"，故可以合并为 "1 2 3"。
*/

#include <bits/stdc++.h>
#include <initializer_list>
#include <vector>
using namespace std;

vector<int> fa;

void init() {
  for (int i = 0; i < fa.size(); ++i) {
    fa[i] = i;
  }
}

int find(int x) {
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}

void unit(int x, int y) {
  fa[find(x)] = find(y);
}

int main() {
  int n; cin >> n;
  cin.ignore();
  int count_id = 0;
  unordered_map<string, int> mp;
  vector<vector<string>> lists(n);
  for (int i = 0; i < n; ++i) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    string tmp;
    while (ss >> tmp) {
      lists[i].emplace_back(tmp);
      if (!mp.count(tmp)) mp[tmp] = count_id++;
    }
  }

  fa.resize(count_id);
  init();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < lists[i].size() - 1; ++j) {
      int x = mp[lists[i][j]];
      int y = mp[lists[i][j + 1]];
      unit(x, y);
    }
  }

  unordered_map<int, vector<string>> groups;
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < lists[i].size(); ++j) {
      int root = find(mp[lists[i][j]]);
      groups[root].emplace_back(lists[i][j]);
    }
  }

  vector<vector<string>> res;
  for (auto [idx, group] : groups) {
    res.emplace_back(group);
  }

  for (auto &v : res) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
  }

  sort(res.begin(), res.end());

  for (int i = 0; i < res.size(); ++i) {
    for (int j = 0; j < res[i].size(); ++j) {
      cout << res[i][j] << " ";
    }
    cout << "\n";
  }

}

