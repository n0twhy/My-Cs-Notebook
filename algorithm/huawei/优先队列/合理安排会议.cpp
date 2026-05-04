/*
题目内容

你是部门秘书，每天你需要根据会议申请表安排会议。

部门所在大楼有 F 层，每层 M 间会议室，每间会议室仅可安排 1 场会议，每场会议耗时 2 单位。

每条会议申请对应 1 场会议并占用 1 间会议室，申请人只会申请自己所在楼层会议室，会议可安排在申请楼层或下方楼层。安排在下方楼层由于人员赶路也会增加耗时，每移动 1 层楼耗时 1 单位。

请根据会议申请表，合理安排会议，让部门会议最高效，即总消耗人时最小。如果申请表无法满足，返回 -1。
每场会议消耗人时 = 申请人数 * (会议耗时 + 移动楼层耗时)。

输入描述

第 1 行是：F M N，
其中 F 为大楼层数，范围为 (0,1000]；
M 为每层会议室间数，范围为 (0,100]；
N 为申请会议表长度，范围为 (0,100000]。

第 2 行到第 N+1 行：
Ri Pi，
其中 Ri 为第 i 条申请的申请会议室楼层，范围为 (0,F]；
Pi 为第 i 条申请的会议人数，范围为 (0,50]。

输出描述

总消耗人时。

样例1
输入
1 1 2
1 10
1 20

输出
-1

说明
部门有 1 层楼，每层 1 间会议室，会议申请表有 2 条申请。
会议室数量无法满足申请表，返回 -1。

样例2
输入
4 1 3
3 10
1 10
3 20

输出
90

说明
部门有 4 层楼，每层 1 间会议室，会议申请表有 3 条申请。

第 1 条会议申请希望在 3 楼开会，10 人参与。
第 2 条会议申请希望在 1 楼开会，10 人参与。
第 3 条会议申请希望在 3 楼开会，20 人参与。

由于有 2 场会议申请在 3 楼，会议室不够分配。

如果将第 1 条会议申请的会议室向下安排到 2 楼，其他会议安排楼层与申请楼层保持不变：
总消耗人时为 10*(2+1) + 10*2 + 20*2 = 90。

如果将第 3 条会议申请的会议室向下安排到 2 楼，其他会议室安排楼层与申请楼层保持不变：
总消耗人时为 10*2 + 10*2 + 20*(2+1) = 100。

故选择将第 1 条会议申请的会议室向下分配到 2 楼更合理。

样例3
输入
3 3 4
2 20
1 10
2 10
2 10

输出
100

说明
部门有 3 层楼，每层 3 间会议室，会议申请表有 4 条申请。

第 1 条会议申请希望在 2 楼开会，20 人参与。
第 2 条会议申请希望在 1 楼开会，10 人参与。
第 3 条会议申请希望在 2 楼开会，10 人参与。
第 4 条会议申请希望在 2 楼开会，10 人参与。

总消耗人时为 20*2 + 10*2 + 10*2 + 10*2。
*/

#include <bits/stdc++.h>
using namespace std;

int main() {
  int f, m, n;
  cin >> f >> m >> n;
  vector<vector<int>> schedule(f + 1);
  for (int i = 0; i < n; ++i) {
    int curr_f, head_cnt;
    cin >> curr_f >> head_cnt;
    schedule[curr_f].emplace_back(head_cnt);
  }

  int res = 0;

  priority_queue<int, vector<int>, less<>> pq;
  int pq_people = 0;

  vector<int> rooms(f + 1, m);
  for (int i = f; i >= 1; --i) {
    for (auto cnt : schedule[i]) {
      pq.push(cnt);
      pq_people += cnt;
    }
    cout << "now, it's floor[" << i << "] pq_people: " << pq_people << endl; 
    while (rooms[i]) {
      if (pq.empty()) break;
      int people = pq.top();
      pq.pop();
      res += people * 2;
      pq_people -= people;
      rooms[i]--;
    }
    res += pq_people;
  }

  if (!pq.empty()) {
    cout << -1 << endl;
    return 0;
  }

  cout << res << endl;
}


/*
#include <bits/stdc++.h>
#include <queue>
#include <vector>
using namespace std;

int main() {
  int f, m, n;
  cin >> f >> m >> n;
  priority_queue<int, vector<int>, less<int>> pq;

  vector<vector<int>> schedule(f + 1);
  for (int i = 0; i < n; ++i) {
    int plan_f, head_cnt;
    cin >> plan_f >> head_cnt;

    schedule[plan_f].emplace_back(head_cnt);
  }

  int res = 0;
  int curr_head_cnt = 0;
  for (int i = f; i >= 1; --i) {
    for (auto cnt : schedule[i]) {
      curr_head_cnt += cnt;
      pq.push(cnt);
    }
    int T = m;
    while (T-- && !pq.empty()) {
      int curr = pq.top();
      pq.pop();
      res += curr * 2;
      curr_head_cnt -= curr;
    }
    if (i > 1) res += curr_head_cnt;
  }
  if (!pq.empty()) {
    cout << -1 << endl;
    return 0;
  }
  cout << res << endl;
}

*/