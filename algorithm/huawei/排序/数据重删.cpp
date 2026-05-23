/*
题目描述
数据重删是一种节约存储空间的技术，通常情况下，在数据存储池内是有很多重复的数据库。
重删则是将这些重复的数据块找出并处理的技术。简单地说重删，就是将 N 份重复的数据块仅保留 1 份，
并将 N−1 份数据的地址指针指向唯一的那一份。

我们输入一串存储的数据，用 N 表示数据个数，用 K 标识每个数据库的大小。
设计一个方法判断当前数据块是否和前面的数据库有重复，两个数据库内容完全一样则表示重复。
如果重复则将这个数据库删除，并且在第一个出现该数据库的后面增加重复数据的计数，
输出经过去重之后的内容。

输入描述
第一行：一个整数 N，表示输入数据的个数。
第二行：一个整数 K，表示数据库的大小。
第三行：N 个整数，依次是数据值。

输出描述
输出去除重复数据块之后的结果。
输出结果为：按照出现顺序依次输出每个保留的数据块及其计数信息，
数字之间用一个空格隔开，最后以数字结尾（末尾没有多余空格）。

说明
按照 K 为窗口大小进行切片，每连续 K 个数表示一个数据块。
若某数据块在之前已出现，则不再单独输出该数据块，而是将其计数加一；
计数紧跟在该数据块最后一个元素之后输出。

样例1
输入
8
2
1 2 3 4 1 2 3 4

输出
1 2 2 3 4 2

解释
总共 8 个数据，数据库的大小为 2，按新窗口进行切片表示一个数据块，
依次得到数据块为 [1,2]、[3,4]、[1,2]、[3,4]。
其中第一个数据块和第三个数据块相同，第二个数据块和第四个数据块相同，
去重之后，[1,2] 的计数变为 2，[3,4] 的计数也变为 2，
最终输出为 1 2 2 3 4 2。

样例2
输入
8
3
3 4 5 3 4 5 5 4

输出
3 4 5 2 5 4 1
*/

#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
  int n, k;
  cin >> n >> k;
  
  string input;
  cin.ignore();
  getline(cin, input);
  stringstream ss(input);
  vector<int> data;
  int tmp;
  while (ss >> tmp) {
    data.emplace_back(tmp);
  }

  int left = 0;
  int right = left + k - 1;
  
  vector<int> ans;
  map<vector<int>, pair<int, int>> mp;

  while (right < data.size()) {
    vector<int> curr;
    for (int i = left; i <= right; ++i) {
      curr.emplace_back(data[i]);    
    }
    if (mp.count(curr)) {
      mp[curr].first++;
    } else {
      mp[curr].first = 1;
      mp[curr].second = left;
    }
    
    left += k;
    right += k;

    if (right >= data.size() && left < data.size()) {
      vector<int> add;
      for (int i = left; i < data.size(); ++i) {
        add.emplace_back(data[i]);
      }
      mp[add].first = 1;
      mp[add].second = left;
    }
  }

  map<pair<int, int>, vector<int>> letsgo;
  for (auto [v, p] : mp) {
    auto [cnt, idx] = p;
    letsgo.insert({{idx, cnt}, v});
  }
  for (auto [p, v] : letsgo) {
    for (int i = 0; i < v.size(); ++i) {
      cout << v[i] << " ";
    }
    cout << p.second << " ";
  }
}

