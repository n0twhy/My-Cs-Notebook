/*
题目描述

小明想要处理一批图片，将相似的图片分类。他首先对图片的特征采样，得到图片之间的相似度，然后按照以下规则判断图片是否可以归为一类：

1. 相似度 > 0 表示两张图片相似；
2. 如果 A 和 B 相似，B 和 C 相似，但 A 和 C 不相似。那么认为 A 和 C 间接相似，可以把 ABC 归为一类，但不计算 AC 的相似度；
3. 如果 A 和所有其他图片都不相似，则 A 自己归为一类，相似度为 0。

给定一个大小为 N×N 的矩阵 M 存储任意两张图片的相似度，M[i][j] 即为第 i 个图片和第 j 个图片的相似度，请按照「从大到小」的顺序返回每个相似类中所有图片的相似度之和。

输入描述

第一行一个数 N（1≤N≤900），代表矩阵 M 中有 N 个图片。下面跟着 N 行，每行有 N 列数据，空格分隔（为了显示整齐，空格可能为多个），代表 N 个图片之间的相似度。

其中 0≤M[i][j]≤100，输入保证 M[i][j]=M[j][i]。

输入的矩阵分隔符为 1 个或多个连续空格。

输出描述

每个相似类的相似度之和。格式为：一行数字，分隔符为 1 个空格。

样例1
输入
5
0 0 50 0 0
0 0 0 25 0
50 0 0 0 15
0 25 0 0 0
0 0 15 0 0

输出
65 25

说明

把 1~5 看成 A,B,C,D,E。
矩阵显示，A 和 C 相似度为 50，C 和 E 的相似度为 15；B 和 D 相似度为 25。
划分出 2 个相似类，分别为：
1. {A,C,E}，相似度之和为 65
2. {B,D}，相似度之和为 25

排序输出相似度之和，结果为：65 25
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

vector<int> fa;

void init() {
  for (int i = 0; i < fa.size(); ++i) {
    fa[i] = i;
  }
}

int find (int x) {
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}

void unit(int x, int y) {
  fa[find(x)] = find(y);
}

int main() {
  int n;
  cin >> n;
  vector<vector<int>> grid(n, vector<int>(n));

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
    }
  }

  fa.resize(n);
  init();

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (grid[i][j] > 0) {
        unit(i, j);
      }   
    }
  }

  unordered_map<int, vector<int>> groups;

  for (int i = 0; i < fa.size(); ++i) {
    int root = find(i);
    groups[root].emplace_back(i);    
  }
  vector<int> res;
  for (auto [idx, v] : groups) {
    int cnt = 0;
    for (int i = 0; i < v.size(); ++i) {
      for (int j = i + 1; j < v.size(); ++j) {
        cnt += grid[v[i]][v[j]];
      }

    }

    res.emplace_back(cnt);
  }

  sort(res.begin(), res.end(), [](int &a, int &b){
    return a > b;
  });
  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
}