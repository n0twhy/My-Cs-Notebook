/*
题目：坐火车

题目内容
火车站有 n 辆火车，编号范围为 [0, n)。
现有 m 名乘客，每名乘客给出要乘坐的火车编号与计划上车时间。

车站需要统计：在指定时间区间内，有乘客上车的火车数量。
时间区间长度固定为 X，给定 K 个起始时间 S1...SK，
对应 K 个区间 [Si, Si+X)。区间之间可能重叠。

请按给定起始时间顺序输出每个区间内“有乘客上车”的火车数量。

输入描述
第 1 行：N X K
- N：火车数量，范围 [1,100000)
- X：时间区间长度，范围 [1,100000]
- K：要统计的区间个数，范围 [1,100000]

第 2 行：S1 S2 ... SK
- 共 K 个起始时间，范围 [0,100000]

第 3 行：M
- M 为乘客数量，范围 [1,100000)

第 4 行到第 M+3 行：每行两个整数 i j
- i：乘客要乘坐的火车编号，范围 [0,n)
- j：乘客计划上车时间，范围 [0,100000)

输出描述
按给定的 K 个起始时间输入顺序，依次输出每个时间区间内有乘客上车的火车数量。

样例1
输入
4 2 2
3 4
4
2 4
2 3
1 2
3 5

输出
1 2

说明
区间为 [3,5) 与 [4,6)：
- 在 [3,5) 中，仅火车 2 有乘客上车（虽然有两名乘客，但只计一辆火车）；
- 在 [4,6) 中，火车 2 和火车 3 都有乘客上车。
故输出 1 2。

样例2
输入
4 5 3
20 10 30
3
3 32
0 14
1 30

输出
0 1 2
*/
/*
#include <bits/stdc++.h>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> Train(vector<int> &starts, int k, int x, vector<pair<int, int>> &passengers) {
    int n = starts.size();
    vector<int> cnts(starts.size());
    vector<pair<int, int>> starts_sort(starts.size());

    for (int i = 0; i < n; ++i) {
      starts_sort[i] = {starts[i], i};
    }

    sort(starts_sort.begin(), starts_sort.end());
    sort(passengers.begin(), passengers.end(), [](pair<int, int> &a, pair<int, int>& b) {
      return a.second < b.second;
    });

    vector<int> count(k, 0);
    int diff = 0;
    int right = 0;
    int left = 0;
    vector<int> ans(n);
    for (auto [start, idx] : starts_sort) {
      int end = start + x;
      
      while (right < passengers.size() && passengers[right].second < end) {
        if (count[passengers[right].first]++ == 0) diff++;
        right++;
      }

      while (left < passengers.size() && passengers[left].second < start) {
        if (--count[passengers[left].first] == 0) diff--;
        left++;
      }

      ans[idx] = diff;
    }

    return ans;
  }
};

int main() {
  int n, x, k;
  cin >> n >> x >> k;

  vector<int> starts(k);
  for (int i = 0; i < k; ++i) {
    cin >> starts[i];
  }

  int m;
  cin >> m;
  vector<pair<int, int>> passengers(m);
  for (int i = 0; i < m; ++i) {
    cin >> passengers[i].first >> passengers[i].second;
  }

  Solution sol;
  auto res = sol.Train(starts, n, x, passengers);

  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
}
*/
#include <bits/stdc++.h>
#include <random>
#include <vector>
using namespace std;

int main() {
  int n, x, k;
  cin >> n >> x >> k;
  
  vector<pair<int, int>> letsgo(k);
  for (int i = 0; i < k; ++i) {
    cin >> letsgo[i].first;
    letsgo[i].second = i;
  }

  int m; cin >> m;
  vector<pair<int, int>> passengers(m);

  for (int i = 0; i < m; ++i) {
    cin >> passengers[i].first >> passengers[i].second;
  }

  sort(letsgo.begin(), letsgo.end());
  sort(passengers.begin(), passengers.end(), [](pair<int, int> &a, pair<int, int> &b) {
    return a.second < b.second;
  });

  vector<int> count(n, 0);
  int diff = 0;
  int left = 0;
  int right = 0;

  vector<int> res(k);

  for (auto [start, idx] : letsgo) {
    int end = start + x;
    
    while (right < passengers.size() && passengers[right].second < end) {
      if (count[passengers[right].first]++ == 0) {
        diff++;
      }
      right++;
    }


    while (left < passengers.size() && passengers[left].second < start) {
      if (--count[passengers[left].first] == 0) {
        diff--;
      }
      left++;
    }

    cout << "now " << start << ": " << end << " and diff: " << diff << endl;

    res[idx] = diff;
  }
  
  for (int i = 0; i < k; ++i) {
    cout << res[i] << " ";
  }

  cout << "\n";
}