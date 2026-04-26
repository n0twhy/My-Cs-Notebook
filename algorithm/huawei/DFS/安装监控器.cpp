/*
题目内容
某数据中心机房内摆放了 M 排 N 列机柜。
现需要在每排选择一个机柜安装监控器，用来监视本排机柜的用电量。
由于监控器在安装位置太近时会产生相互干扰，安装时需满足：
- 安装监控器的机柜不能在同一列；
- 不能在同一条斜线上（45° 或 135° 方向的正斜线，即棋盘上的两条对角线方向）。

问一共有多少种监控器安装方案。

输入描述
两个整数 M、N，分别表示机柜的排数与列数。
取值范围：1 <= M, N <= 15（题面写作 [1,15)，按常见题意理解为不超过 15 的正整数）。

输出描述
输出监控器安装方案的数量；若无合法方案则输出 0。

样例1
输入
2 3

输出
2

说明
第二排监控器需安装在第一排监控器相距 2 列以上的机柜，共有 2 种安装方案。
图中 0 表示未安装监视器的机柜，1 表示安装监控器的机柜。
（题面附图略）

样例2
输入
3 3

输出
0

说明
无法同时满足：各排各选一个、且任意两个不在同一列、同一斜线上。
（题面附图略）
*/
#include <bits/stdc++.h>
#include <functional>
#include <vector>
using namespace std;

class Solution {
public:
  int Nqueen(int m, int n) {
    vector<bool> cols(n, false);
    vector<bool> diag_1(n + m - 1, false);
    vector<bool> diag_2(m + n - 1, false);
    int cnt = 0;

    function<void(int)> dfs = [&](int start) {
      if (start == m) {
        cnt++;
        return;
      }
      for (int j = 0; j < n; ++j) {
        if (cols[j]) continue;
        int sum = start + j;
        int diff = start - j + n - 1;
        if (diag_1[sum]) continue;
        if (diag_2[diff]) continue;
        
        cols[j] = true;
        diag_1[sum] = true;
        diag_2[diff] = true;
        dfs(start + 1);

        cols[j] = false;
        diag_1[sum] = false;
        diag_2[diff] = false;
      }
    };
    dfs(0);
    return cnt;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int m, n;
  cin >> m >> n;
  Solution sol;
  cout << sol.Nqueen(m, n) << endl;
}