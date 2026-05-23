/*
题目内容

你有一个通讯录，这个通讯录里每个联系人都包含姓名和手机号，一个联系人可能有多个手机号。

如果发现两个联系人拥有相同的手机号，我们就认为他们是同一个人。

你的任务就是整理这个通讯录，将具有相同手机号的联系人合并为一个联系人，并返回合并后的通讯录列表。

注意：用例不保证具有相同手机号的联系人姓名肯定是相同的，如果合并的时候发现联系人姓名不同，那么以字典序小的为姓名。

输入描述

第一行表示通讯录的记录数量 num，值的范围 [1,1000]。

从第二行开始，每一行代表一条联系人记录，每条记录包括姓名和若干电话号码，电话号码的个数不超过 10（姓名由英文字母大小写组成，长度在 1 到 10 之间，电话号码由数字组成）。

每条记录中电话号码个数在 1～10 之间；电话号码为数字串，长度在 [1,10]；同一条记录内的号码互不重复。号码之间用空格隔开。

输出描述

整理后的通讯录列表，其中如果两个联系人被识别为同一个人，则他们的电话号码合并。

输出联系人姓名和手机号码列表按 ASCII 码升序排序。

样例1
输入
4
kaka 10000000000 10000000001
tata 10000000020
kaka 10000000000 10000000002
tata 10000000010

输出
kaka 10000000000 10000000001 10000000002
tata 10000000010
tata 10000000020

说明

在这个通讯录中，第一个 kaka 和第二个 kaka 有相同的手机号「10000000000」，因此他们被视为同一个人。

因此 kaka 一共有三个号码并按 ASCII 码升序排序结果是 10000000000 10000000001 10000000002。

第一个 tata 和第二个 tata 没有相同的号码，因此被视作两个人。

样例2
输入
3
kaka 10000000000 10000000001
kaka 10000000001 10000000002
kaka 10000000002 10000000003

输出
kaka 10000000000 10000000001 10000000002 10000000003

说明

在这个通讯录中，第一个 kaka 和第二个 kaka 有相同的手机号「10000000001」，第二个 kaka 和第三个 kaka 有相同的手机号「10000000002」，因此三者被视为同一个人。

因此 kaka 一共有四个号码并按 ASCII 码升序排序结果是 10000000000 10000000001 10000000002 10000000003
*/

#include <bits/stdc++.h>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

vector<int> parent;

void init() {
  for (int i = 0; i < parent.size(); ++i) {
    parent[i] = i;
  }
}

int find(int x) {
  return parent[x] == x ? x : parent[x] = find(parent[x]);
}

void unit(int x, int y) {
  parent[find(x)] = find(y); 
}

int main() {
  int n; cin >> n;
  int count_id = 0;
  unordered_map<string, int> mp;
  vector<vector<string>> list(n);
  cin.ignore();
  for (int i = 0; i < n; ++i) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    string name;
    ss >> name;
    list[i].emplace_back(name);
    string number;
    while (ss >> number) {
      if (!mp.count(number)) {
        mp[number] = count_id++;
      }
      list[i].emplace_back(number);
    }
  }
  
  parent.resize(count_id);
  init();

  for (auto v : list) {
    for (int i = 1; i < v.size() - 1; ++i) {
      unit(mp[v[i]], mp[v[i + 1]]);
    }
  }
  
  unordered_map<int, pair<string, set<string>>> group;
  vector<vector<string>> res;
  for (int i = 0; i < (int)list.size(); ++i) {
    for (int j = 1; j < (int)list[i].size(); ++j) {
        //cerr << "i=" << i << " j=" << j << " num=" << list[i][j] << flush;
        int id = mp[list[i][j]];
        //cerr << " id=" << id << flush;
        int root = find(id);
        //cerr << " root=" << root << flush;
        auto &g = group[root];
        //cerr << " got_g" << flush;
        if (g.first.empty()) g.first = list[i][0];
        else g.first = min(g.first, list[i][0]);
        //cerr << " set_name" << flush;
        g.second.insert(list[i][j]);
        //cerr << " inserted" << endl;
    }
}
//cerr << "loop done" << endl;
  for (auto &[idx, p] : group) {
    auto &[name,numbers] = p;
    vector<string> tmp;
    tmp.emplace_back(name);
    for (auto number : numbers) {
      tmp.emplace_back(number);
    }
    res.emplace_back(tmp);
  }

  sort(res.begin(), res.end());

  for (int i = 0; i < res.size(); ++i) {
    for (int j = 0; j < res[i].size(); ++j) {
      cout << res[i][j] << " ";
    }
    cout << "\n";
  }
}