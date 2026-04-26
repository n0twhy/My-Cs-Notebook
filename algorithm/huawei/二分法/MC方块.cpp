/*
题目：MC方块

题目内容
MC 最新版本更新了一种特殊方块：幽匿催发体。
这种方块能够吸收生物死亡掉落的经验，并感染周围方块，使其变成幽匿块。

在 Steve 的实验中，世界是由草方块组成的超平坦平面。
幽匿催发体可看作：每次吸收经验后，会向平面方向上的周围八个方块扩散一圈感染范围。

Steve 任意选择了 n 个坐标点作为幽匿催发体的起始方块。
此后每天都会给予这些催发体足够经验，使其感染范围向外扩展一圈。

当两个或以上幽匿催发体的感染范围重叠时，重叠区域方块会吸收更多经验：
某个方块的经验倍数等于覆盖该方块的不同催发体感染范围数量。

Steve 想知道：多少天以后，会出现至少一个方块的经验倍数达到给定 M？

输入描述
第一行输入整数 M（2 <= M <= n）。
第二行输入幽匿催发体个数 n（2 <= n <= 50）。
接下来 n 行，每行输入第 i 个幽匿催发体初始位置 [xi, yi]（1 <= xi, yi <= 10^9）。

输出描述
输出最少天数，使得存在一个方块同时处在至少 M 个幽匿催发体的感染范围中。
若找不到则输出 0。

样例1
输入
2
2
2 1
6 2

输出
2

说明
在第 2 天，点 (4,0)、(4,1)、(4,2)、(4,3) 同时处在两个幽匿催发体的感染范围内。

样例2
输入
2
3
2 1
6 2
100 100

输出
2
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int CheckCnt(int mid, vector<pair<int, int>> &positions) {
    int cnt = 0;
    int length = 2 * mid;
    for (auto &[x_1, y_1] : positions) {
      for (auto &[x_2, y_2] : positions) {
        if (x_1 == x_2 && y_1 == y_2) continue;
        if (abs(x_1 - x_2) < length && abs(y_1 - y_2) < length) cnt++;
      }
    }
    return cnt;
  }

  int MCSquare(int value, vector<pair<int, int>> &positions) {
    int left = 1;
    int right = INT_MAX;
    int res = INT_MAX;

    while (left <= right) {
      int mid = (left + right) / 2;
      int m = CheckCnt(mid, positions);

      if (m > value) {
        res = min(res, mid);
        right = mid - 1;
      } else {
        left = mid + 1;
      }
    }
    return res;
  }
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<pair<int, int>> positions(n);
  for (int i = 0; i < n; ++i) {
    cin >> positions[i].first >> positions[i].second;
  }
  Solution sol;
  cout << sol.MCSquare(m, positions) << endl;
}