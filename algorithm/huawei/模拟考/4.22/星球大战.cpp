/*
题目内容
在潘多拉星球上，有一群怪兽组成一道阵线，奥特曼必须按顺序与这些怪兽战斗。奥特曼有一个初始能量值 E，每个怪兽都有一个攻击力 damage 和击败奖励 reward（击败后奥特曼可以恢复相应的能量值）。

当奥特曼面对第 i 个怪兽时，有以下选择：

如果当前能量值大于怪兽攻击力 damage[i]，奥特曼可以选择战斗，此时会先消耗掉 damage[i] 点能量，然后增加 reward[i] 点能量；

如果当前能量值小于或等于怪兽攻击力 damage[i]，奥特曼不能与该怪兽战斗（因为奥特曼剩余能量不能 <= 0），此时只能跳过该怪兽；

无论能否打过，都选择跳过该怪兽，此时奥特曼不消耗也不增加能量；

奥特曼的目标是尽可能多的击败怪兽（即最大化击败数量），现在需要计算奥特曼最多能击败多少个怪兽。

注意：
奥特曼与怪兽战斗的顺序不能改变。

约束条件：
1 <= len(damage) = len(reward) <= 100
1 <= E <= 10^9
1 <= damage[i], reward[i] <= 10^9

输入描述
E: 整数，奥特曼初始能量值。
damage: 整数数组，每个怪兽的攻击力值。
reward: 整数数组，击败每个怪兽后获得能量值奖励。

输出描述
整数，最多击败的怪兽数量。

样例1
输入
18
15 17 4 18
1 15 4 17

输出
2

说明
奥特曼初始能量为 18，然后按顺序与怪兽战斗：

跳过第 1 个怪兽；
打第 2 个怪兽：奥特曼当前能量 18，怪兽攻击力 17，奖励 15，奥特曼剩余能量：18 - 17 + 15 = 16；
打第 3 个怪兽：怪兽攻击力 4，奖励 4，奥特曼剩余能量：16 - 4 + 4 = 16；
奥特曼剩余能量小于 18，所以跳过第 4 个怪兽；最多打败 2 个怪兽，输出 2。

样例2
输入
5
10 20 30
1 1 1

输出
0

说明
每个怪兽都打不过，输出 0。

样例3
输入
9
5 4 5
0 3 4

输出
2

说明
奥特曼初始能量为 9，然后按顺序与怪兽战斗：

跳过第 1 个怪兽；
打第 2 个怪兽：奥特曼当前能量 9，怪兽攻击力 4，奖励 3，奥特曼剩余能量：9 - 4 + 3 = 8；
打第 3 个怪兽：怪兽攻击力 5，奖励 4，奥特曼剩余能量：8 - 5 + 4 = 7；
最多打败 2 个怪兽，输出 2。
*/

#include <bits/stdc++.h>
#include <vector>
#define int long long
using namespace std;

signed main() {
  int MAX_NUM = 0x3f3f3f3f;
  int E; cin >> E;
  cin.ignore();
  vector<int> monsters;
  vector<int> reward;
  string input;
  getline(cin, input);
  stringstream ss(input);
  int tmp;
  while (ss >> tmp) {
    monsters.emplace_back(tmp);
  }
  getline(cin, input);
  stringstream ss_2(input);
  while (ss_2 >> tmp) {
    reward.emplace_back(tmp);
  }
  int n = monsters.size();
  vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
  dp[0][0] = E;
  for (int i = 1; i <= n; ++i) {
    for (int j = 0; j <= n; ++j) {
      if (dp[i - 1][j] == -1) continue;
      dp[i][j] = max(dp[i][j], dp[i - 1][j]);

      if (j + 1 <= n && dp[i - 1][j] > monsters[i - 1]) {
        dp[i][j + 1] = max(dp[i][j + 1], dp[i - 1][j] - monsters[i - 1] + reward[i - 1]);
      }
    }
  }
  int ans = 0;
  for (int i = 0; i <= n; ++i) {
    if (dp[n][i] != -1) {
      ans = max(ans, i);
    }
  }
  cout << ans << endl;
}