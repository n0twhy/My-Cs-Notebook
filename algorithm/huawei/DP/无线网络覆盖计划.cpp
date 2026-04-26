/*
题目内容
你正在设计一个大型无线网络覆盖计划，目标是通过布置多个无线接入点来覆盖一个区域。
每个接入点都有不同的信号覆盖范围和安装成本。
在预算有限的情况下，保证网络覆盖需求满足，并且总成本不超过预算。

约束条件：
1. 信号覆盖范围：每个接入点能够覆盖一定的区域，区域面积单位为 m^2。
2. 安装成本：每个接入点有一定的安装成本，单位为元，总成本不应超过预算。
3. 区域需求：已知整个区域的总覆盖面积需求，单位为 m^2。

输入描述
1、第一行包含三个数字：
- areaRequirement：所需的区域覆盖面积（单位：m^2，0 < areaRequirement <= 100000）
- budget：总预算（单位：元，0 < budget <= 10000，且 budget 为 10 的整数倍）
- n：接入点的数量（0 < n <= 10000）

2、接下来的 n 行每行包含两个数字，分别是：
- coverage：接入点的信号覆盖范围（单位：m^2，0 < coverage <= 100000）
- cost：接入点的安装成本（单位：元，0 < cost <= 100000，且 cost 为 10 的整数倍）

输出描述
1、输出在给定成本内能满足区域覆盖需求的最小预算以及此时的覆盖面积。
   如果有多个解预算都能满足覆盖范围要求，输出预算最小时最大的覆盖面积。
2、如果给出的站点无法满足要求则输出：
0 0

样例1
输入
2000 500 3
1000 200
1500 250
800 180

输出
430 2300

说明
第一行表示：目标是需要一个信号覆盖范围至少为 2000 m^2 的区域，总预算为 500 元，
共有 3 个接入点可供选择。

接入点1：覆盖范围 1000 m^2，成本 200 元
接入点2：覆盖范围 1500 m^2，成本 250 元
接入点3：覆盖范围 800 m^2，成本 180 元

可选接入点2和接入点3，成本最低为 430 元，覆盖范围为 2300 m^2。

样例2
输入
3000 500 3
1000 200
1500 250
800 180

输出
0 0

说明
无论选择哪些接入点，都无法在不超过预算 500 的情况下满足 3000 m^2 的区域覆盖需求，
所以输出 0 0。
*/
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  pair<int, int> WIFICover(int area, int budget, vector<pair<int, int>> &devices) {
    vector<int> dp(budget + 1, 0);
    for (auto &[area_, price] : devices) {
      for (int i = budget; i >= price; --i) {
        dp[i] = max(dp[i], dp[i-price] + area_);
      }
    }

    for (int i = 0; i <= budget; ++i) {
      if (dp[i] >= area && dp[i - 1] < area) {
        return {i, dp[i]};
      }   
    }

    return {0, 0};
  }

};

int main() {
  int area, budget, cnt;
  cin >> area >> budget >> cnt;
  vector<pair<int, int>> devices(cnt);
  for (int i = 0; i < cnt; ++i) {
    cin >> devices[i].first >> devices[i].second;
  }
  Solution sol;
  auto ans = sol.WIFICover(area, budget, devices);
  cout << ans.first << " " << ans.second << endl;

}