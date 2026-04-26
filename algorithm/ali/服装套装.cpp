/*
题目内容
节日临近，某时装店需要安排当日陈列与销售。仓库现有：领带 a 条、围巾 b 条、夹克 c 件。商店出售如下两类套装：

第一类套装：
1 条领带 + 1 件夹克，售价 d 金币；

第二类套装：
1 条围巾 + 1 件夹克，售价 e 金币。

商店每天的陈列展位共计 r 个，其中第一类套装占用 1 个展位，第二类套装占用 2 个展位。每件服装至多参与一个套装，允许有剩余不使用。请计算在不超过展位限制的前提下，最多可以获得的总收益。

输入描述
每个测试文件均包含多组测试数据。第一行输入一个整数 T(1 <= T <= 10^5) 表示数据组数。

每组测试数据一行输入六个整数 a, b, c, d, e, r(0 <= a, b, c, d, e, r <= 10^9)。

输出描述
对于每组测试数据，输出一行，包含一个整数，表示在最优方案下的最大总收益（单位：金币）。

样例1
输入
3
3 0 1 3 5 3
10 10 10 10 8 11
4 1 1 3 10 3
输出
3
100
10
*/

#include <bits/stdc++.h>
#include <functional>
#include <utility>
#include <vector>
using namespace std;

#define int long long

class Solution {
public:
  int MaximalProfit() {
    int a, b, c, d, e, r;
    cin >> a >> b >> c >> d >> e >> r;

    int x_max = min({a, c, r});
    vector<int> candidates = {
      0,
      x_max,
      r - 2 * b,
      2 * c - r,
      c - b
    };
    int res = 0;
    for (auto x0 : candidates) {
      for (int dx = -2; dx <= 2; ++dx) {
        int x = x0 + dx;
        if (x < 0 || x > x_max) continue;
        
        int y = min({b, (r - x) / 2, c - x});
        res = max(res, d * x + e * y);
      }
    }

    return res;
  }
};

signed main() {
  int n;
  cin >> n;
  Solution sol;
  for (int i = 0; i < n; ++i) {
    cout << sol.MaximalProfit() << endl;
  }
}