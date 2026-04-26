/*
题目内容

在《永劫无间》的决赛圈中，作为仅存的侠客之一，你正处于“聚窟洲”的一处狭长索桥上与其余敌方侠客对峙。

此时“暗域”（毒圈）已经严重蔓延，索桥的左端（坐标 <= 0）与右端（坐标 >= L）均已被高浓度的暗域覆盖，任何侠客一旦进入暗域范围就会因体力耗尽被瞬间淘汰。

令索桥的安全区域左侧边缘对应坐标 x=0，右侧边缘对应坐标 x=L。当前桥上还有 n 名敌方侠客，其初始坐标为 x1, x2, …, xn，同一位置上可能有多名侠客，但均满足（0 < xi < L），即暂时处于安全区内。

你捡到了一把金色品质的“火炮”，并装备了稀有魂玉“连珠·反弹”。你可以向索桥上任意一个整数坐标点 d 发射一枚具有强力冲击波的炮弹，产生如下效果：

1. 命中：如果某名敌方侠客当前坐标恰好为 d，则被炮弹直接命中造成巨额伤害淘汰；
2. 左击退：如果某名敌方侠客当前坐标为 y 且 y < d，则会被爆炸气浪向左震飞 r 个单位，变为坐标 y-r；
3. 右击退：如果某名敌方侠客当前坐标为 y 且 y > d，则会被爆炸气浪向右震飞 r 个单位，变为坐标 y+r。

当敌方侠客的坐标被震飞至 <= 0 或 >= L 时，就会跌入暗域被淘汰。

为了拿下“不朽荣光”成就（在另外两名队友已经阵亡的情况下，独自存活至少 3 分钟并最终获胜），你需要计算至少发射多少次炮弹，才能将所有敌方侠客淘汰？每次开火前，你都可以根据当前幸存敌人的位置，重新选择最优的落点 d。

输入描述

第一行输入三个整数 n, L, r (1<=n<=2×10^5, 2<=L<=10^9, 1<=r<=10^9)，分别表示敌方侠客数量、索桥安全区长度与震飞距离；
第二行输入 n 个整数 x1, x2, …, xn (0<xi<L)，表示初始时每名敌方侠客的坐标。

输出描述

输出一个整数，表示至少需要发射多少次炮弹，才能将所有敌方侠客淘汰。

样例1
输入
3 10 2
3 5 9

输出
2

说明
在这个样例中，一种最优策略是：
第一次瞄准 d=5 处开火，直接淘汰坐标 5 的敌人，其余敌人被震飞至坐标 3-2=1（存活）和 9+2=11（掉入暗域淘汰）；
第二次瞄准 d=1 处开火，淘汰剩余敌人。

样例2
输入
5 20 3
2 6 9 12 17

输出
2

说明
在这个样例中，一种最优策略是：
第一次瞄准 d=9 处开火，直接淘汰坐标 9 的敌人，其余被震飞至 {-1,3,15,20}，其中 -1 和 20 触发暗域淘汰；
第二次瞄准 d=15 处开火，直接淘汰坐标 15 的敌人，另一名被震飞至 0（掉入暗域淘汰）。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

bool check(vector<int> &pos, int k, int r, int L) {
  auto begin = upper_bound(pos.begin(), pos.end(), k * r);
  auto end = lower_bound(pos.begin(), pos.end(), L - k * r);

  return end - begin <= k;
}

int main() {
  int n, L ,r;
  cin >> n >> L >> r;
  vector<int> players(n);

  for (int i = 0; i < n; ++i) {
    cin >> players[i];
  }

  sort(players.begin(), players.end());
  players.erase(unique(players.begin(), players.end()), players.end());

  int left = 1;
  int right = players.size();
  int res = 0x3f3f3f3f;

  while (left <= right) {
    int mid = (left + right) / 2;
    if (check(players, mid, r, L)) {
      res = min(res, mid);
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }

  cout << res << endl;

}