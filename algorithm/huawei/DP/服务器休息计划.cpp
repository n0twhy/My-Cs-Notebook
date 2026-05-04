/*
题目内容
小明计划在假期安排一次自驾旅行。从小明所在的城市，到旅行目的地，仅有一条高速公路。
该高速公路上有 N 个服务区，每个服务区都提供了餐饮、休息等服务，需要一定的花费。
为了避免疲劳驾驶，每经过 M 个服务区，至少必须进入其中的某个服务区停车休息一次。
休息时，需要支付对应服务区的费用。
请帮小明安排一个服务区休息计划，使其在服务区的总花费最少。

输入描述
第一行：两个整数 N、M。
- N 表示服务区的数量。
- M 表示经过连续 M 个服务区，必须至少停车休息一次。

第二行：包含 N 个整数的数组。
第 i 个元素表示在第 i 个服务区休息时需要的费用（假定在服务区 i 停车休息，一定会消费该服务区对应的费用）。

参数范围：
0 < N <= 10000
0 < M <= 50

输出描述
输出一行，表示在服务区休息的最小总花费。

样例1
输入
5 3
5 6 9 10 6

输出
9

说明
在第 3 个服务区休息一次，花费为 9，最少。

样例2
输入
4 2
3 2 2 5

输出
4

说明
在第 2 个服务区和第 3 个服务区休息，总花费为 2 + 2 = 4，最少。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  int n, m; cin >> n >> m;
  vector<int> stations(n);
  for (int i = 0; i < n; ++i) {
    cin >> stations[i];
    cout << stations[i] << " ";
  }

  vector<int> dp(n, 0x3f3f3f3f);
  
  for (int i = 0; i < n; ++i) {
    if (i < m) {
      dp[i] = stations[i];
      continue;
    }
    
    for (int j = 1; j <= m; ++j) {
      dp[i] = min(dp[i], dp[i - j] + stations[i]);
    }
  }

  for (int i = 0; i < n; ++i) {
    cout << dp[i] << " ";
  }
  cout << "\n";
  

  int ans = 0x3f3f3f3f;
  for (int i = 1; i <= m; ++i) {
    ans = min(ans, dp[n - i]);
    cout << "dp[" << n - i << "]: " << dp[n - i] << endl;
    cout << ans << endl;
  }
  cout << ans << endl;
}


/*
#include <bits/stdc++.h> 
#include <climits>
using namespace std;

class Solution {
public:
  int minimalconsume(vector<int> &servers, int lap) {
    vector<int> dp(servers.size(), servers[0]);
    dp[0] = 0;
    for (int i = 1; i <= lap; ++i) {
      dp[i] = servers[i];
    }
    for (int i = lap + 1; i <= servers.size(); ++i) {
      int min_cost = INT_MAX;
      for (int j = 1; j <= lap; ++j) {
        min_cost = min(min_cost, dp[i - j]);
      }
      dp[i] = min_cost + servers[i];
    }
  int min_cost = INT_MAX;
  for (int j = 1; j <= lap; ++j) {
    min_cost = min(dp[servers.size() - j], min_cost);
  }
  return min_cost;
  }
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<int> servers(m);
  for (int i = 0; i < m; ++i) {
    cin >> servers[i];
  }
  Solution sol;
  cout << sol.minimalconsume(servers, n);
}

*/