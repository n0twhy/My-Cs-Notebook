/*
题目内容
有一些网络设备，其中某些设备具备互斥特性：
这些设备中任意两台设备都无法直接或间接地连接在一起。

网络工程师按照给定的一系列指令将设备两两连接。
一旦某条连接指令触发了互斥规则，工程师将拒绝执行这条指令、记录该指令，
并继续执行后续指令。

求所有指令执行完毕后，被拒绝执行指令的总数。

输入描述
输入格式如下：

N M X
A1 ... Am
C1 D1
C2 D2
...
Cx Dx

说明：
Line1：
- N：设备总量（1 <= N <= 1000，设备编号从 0 开始）
- M：互斥设备数量（2 <= M <= 20）
- X：连接指令数量（1 <= X <= 1000）
固定 3 个整数，空格分隔。

Line2：
- 互斥设备编号列表，数量等于 M，且不重复。
- Ai 表示互斥设备编号。

Line3 ~ Line(X+2)：
- 连接指令列表，共 X 条。
- 每条指令包含两个整数 Cj、Dj，表示要连接的两个设备编号。
- 指令内两个编号不重复（Cj != Dj）。

输出描述
输出被拒绝执行的指令条数。

样例1
输入
5 2 5
1 2
0 1
1 2
2 3
0 3
1 4

输出
2
*/

#include <bits/stdc++.h>
using namespace std;

vector<int> fa;

void init(vector<int> &fa) {
  for (int i = 0; i < fa.size(); ++i) {
    fa[i] = i;
  }
}

int find(int x) {
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}

void unit(int x, int y) {
  fa[find(x)] = find(y);
}

int main() {
  int n, m, x;
  cin >> n >> m >> x;

  vector<bool> has_mutex(n, false);
  for (int i = 0; i < m; ++i) {
    int tmp;
    cin >> tmp;
    has_mutex[tmp] = true;
  }

  fa.resize(n);
  init(fa);

  int cnt = 0;

  while (x--) {
    int x, y;
    cin >> x >> y;

    auto rootx = find(x);
    auto rooty = find(y);
    cout << x << " " << y << " : " << rootx << " " << rooty << endl;
    if (rootx != rooty) {
      if (has_mutex[rootx] && has_mutex[rooty]) {
        cnt++;
      } else {
        unit(x, y);
        has_mutex[rooty] = has_mutex[rooty] || has_mutex[rootx];
      }
    }
  }

  cout << cnt << endl;
  
}


/*
#include <bits/stdc++.h>
#include <cstddef>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  int rejectcnt(int n, vector<int> &mutex_nodes, vector<pair<int, int>> & commands) {
    fa.resize(n);
    has_mutex.assign(n, false);

    for (int i = 0; i < n; ++i) {
      fa[i] = i;
    }

    for (int m_mode : mutex_nodes) {
      has_mutex[m_mode] = true;
    }

    int cnt = 0;

    for (const auto [u, v] : commands) {
      int rootU = find(u);
      int rootV = find(v);

      if (rootU != rootV) {
        if (has_mutex[rootU] && has_mutex[rootV]) {
          cnt++;
        } else {
          fa[rootU] = fa[rootV];
          has_mutex[rootV] = has_mutex[rootV] || has_mutex[rootU];
        }
      } 
    }
    return cnt;
  }
private:
  vector<int> fa;
  vector<bool> has_mutex;

  int find(int x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
  }

};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, x;
  if (!(cin >> n >> m >> x)) return 0;

  vector<int> mutex_nodes(m);
  for (int i = 0; i < m; ++i) {
    cin >> mutex_nodes[i];
  }
  vector<pair<int, int>> commands(x);
  for (int i = 0; i < x; ++i) {
    cin >> commands[i].first >> commands[i].second;
  }
  Solution sol;
  cout << fixed << setprecision(1) << double(sol.rejectcnt(n, mutex_nodes, commands));
}
*/