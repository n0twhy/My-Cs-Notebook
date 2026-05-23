/*
题目：电商购物记录

题目内容
某电商平台希望分析用户的购买行为，找出经常一起购买的商品对（商品对指两个不同商品 ID 的商品在同一用户的购物记录中同时出现）。

给定多个用户的购物记录，请找出满足共现频率要求的商品对的数量。

输入描述
第一行：两个整数 N, Q
- N：用户数量（1 <= N <= 1000）
- Q：查询数量（1 <= Q <= 200）

接下来 N 行，每行描述一个用户的购物记录：
- 第一个整数 k：该用户购买的商品数量（1 <= k <= 100）
- 接下来 k 个整数：商品 ID（1 <= 商品 ID <= 10000）

接下来 Q 行，每行一个整数 T：
- 查询共现频率 >= T 的商品对数量

输出描述
共 Q 行，每行输出一个整数：满足条件的商品对数量。

样例1
输入
5 2
3 1 2 3
1 4
2 2 4
1 5
2 1 3
1
2

输出
4
1

说明
商品对共现统计：
(1,2): 1 次
(1,3): 2 次（用户 1 和用户 5）
(2,3): 1 次
(2,4): 1 次

查询：
T=1：共现 >= 1 次的有全部 4 对 -> 4 个
T=2：共现 >= 2 次的有 (1,3) -> 1 个

说明：用户 2、4 只购买了 1 个商品，不生成商品对。

样例2
输入
3 1
5 1 1 2 2 3
3 1 2 4
2 1 3
1

输出
5

说明
购物记录 1：5 1 1 2 2 3，存在重复商品 ID，去重后的商品 ID 为：1 2 3

商品对共现统计：
(1,2): 用户 1, 用户 2 -> 2 次
(1,3): 用户 1, 用户 3 -> 2 次
(2,3): 用户 1 -> 1 次
(1,4): 用户 2 -> 1 次
(2,4): 用户 2 -> 1 次

所有商品对共 5 对。

查询：
T=1：共现 >= 1 次的有 5 个

样例3
输入
3 3
5 1 1 2 2 3
3 1 2 4
2 1 3
2
1
3

输出
1
7
0

说明
商品对共现统计：
(1,2): 1 次
(1,3): 2 次
(1,5): 1 次
(2,3): 1 次
(2,4): 1 次
(3,4): 1 次
(3,5): 1 次

所有商品对共 7 对。

查询：
T=2：共现 >= 2 次的有 (1,3) -> 1 个
T=1：共现 >= 1 次的有全部 7 对 -> 7 个
T=3：共现 >= 3 次的无 -> 0 个
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  int n, q;
  cin >> n >> q;
  map<pair<int, int>, int> mp;
  while (n--) {
    int k; cin >> k;
    vector<int> list(k);
    for (int i = 0; i < k; ++i) {
      cin >> list[i];
    }
    sort(list.begin(), list.end());
    list.erase(unique(list.begin(), list.end()), list.end());
    for (int i = 0; i < list.size(); ++i) {
      for (int j = i + 1; j < list.size(); ++j) {
        int first = list[i];
        int second = list[j];
        if (!mp.count({first, second})) mp.insert({{first, second}, 1});
        else mp[{first, second}]++;
        //cout << "mp[" << first << ", " << second << "]: " << mp[{first, second}] << endl;
      }
    }
  }
  vector<int> count;
  for (auto [p, cnt] : mp) {
    count.emplace_back(cnt);
  }
  sort(count.begin(), count.end());
  while (q--) {
    int query;
    cin >> query;
    auto it = lower_bound(count.begin(), count.end(), query);
    if (it == count.end()) {
      cout << 0 << endl;
      continue;
    }
    int idx = it - count.begin();
    cout << count.size() - idx << endl;
  }
}