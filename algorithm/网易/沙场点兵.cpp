/*
题目内容

在网易旗舰级武侠游戏《逆水寒》的“宋辽边境”战场玩法中，宋辽两军正隔河对峙。

辽军统帅为了挫败我方士气，摆下了“连环阵”：他们派出了 n 支精锐的先锋小队，并公开了这 n 支小队出战的先后顺序以及每一支小队的“战力值”。

作为宋军的战术指挥官，你手下同样拥有 n 支蓄势待发的铁骑小队，每支小队的“战力值”你也了如指掌。根据战场规则，两军将进行 n 轮一对一的“阵前对决”：

1. 每一轮，双方各派出一支小队进行厮杀；
2. 战力值较高的小队将全歼对手并获胜（积 1 分）；
3. 若战力值相同，由于辽军占据地利，判定辽军获胜（宋军积 0 分，辽军积 1 分）。

战国时，齐威王与田忌赛马的故事广为人知，你也想成为如田忌一般的智将，带领你麾下的将士赢得胜利。请判断，是否存在一种排兵布阵的策略，使得我军最终的胜场数严格大于辽军的胜场数？

输入描述

每个测试文件均包含多组测试数据。第一行输入一个整数 T (1 < T ≤ 2×10^5) 表示演练的数据组数。每组测试数据描述如下：

第一行一个整数 n (1 ≤ n ≤ 10^5，n 为奇数)，表示双方派出的小队数量。

第二行包含 n 个整数 a1, a2, …, an (1 ≤ ai ≤ 10^9)，表示我军（宋军）每支小队的战力值。

第三行包含 n 个整数 b1, b2, …, bn (1 ≤ bi ≤ 10^9)，表示敌军（辽军）的出战顺序及其战力值。

保证所有测试中 n 的总和不超过 2×10^5。

输出描述

输出 T 行。对每组数据，若通过合理排兵布阵能使得我军胜场数 > 败场数，输出 YES；否则输出 NO。

样例1
输入
2
5
2 5 7 1 6
3 5 6 2 7
3
3 3 3
3 3 3

输出
YES
NO

说明

对于第一组测试数据，一种最佳出兵顺序如下：

第一阵：派战力 5 迎战辽军 3（胜）；
第二阵：派战力 6 迎战辽军 5（胜）；
第三阵：派战力 7 迎战辽军 6（胜）；
第四阵：派战力 1 迎战辽军 2（败）；
第五阵：派战力 2 迎战辽军 7（败）；

最终 3 胜 2 负，大破辽军，输出 YES。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  bool Combat(vector<int> &song, vector<int> &liao) {
    sort(song.begin(), song.end());
    sort(liao.begin(), liao.end());
    int song_weak = 0, song_strong = song.size() - 1;
    int win_cnt = 0;
    for (int i = liao.size() - 1; i >= 0; --i) {
      if (song[song_strong] > liao[i]) {
        song_strong--;
        win_cnt++;
      } else {
        song_weak++;
      }
    }
    
    if (win_cnt > song.size() / 2) {
      return true;
    } else {
      return false;
    }
  }
};

int main() {
  int T;
  cin >> T;
  Solution sol;
  while (T--) {
    int n;
    cin >> n;
    vector<int> song(n);
    vector<int> liao(n);
    for (int i = 0; i < n; ++i) {
      cin >> song[i];
    }
    for (int i = 0; i < n; ++i) {
      cin >> liao[i];
    }  
    if (sol.Combat(song, liao)) {
      cout << "YES" << endl;
    } else {
      cout << "NO" << endl;
    }
  }
}