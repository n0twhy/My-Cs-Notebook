/*
题目内容

快过年了，小明想要用工作攒下的钱 m 做一点生意补贴家用。和小卖部老板达成协商，可以按成本价格给他提供 n 种商品，让他到隔壁村去销售，其中商品 i 的成本价为 cost[i]，利润为 profit[i]。

由于小明没法携带太多商品，老板也不想在过年期间工作，所以小明每天最多能从老板那里进 1 次货，每次买 1 件商品，购买后的商品无法再次购买。

老板不接收赊账，每种商品只有 1 件库存，请问小明在 k 天中最多可以赚多少利润呢？

输入描述

共 4 行：

第一行 k：表示小明可以做生意的天数，1<=k<=1000

第二行 m：表示小明初始攒的钱，0<=m<=1000

第三行 cost：表示每种商品的成本价，0<=cost[i]<=1000

第四行 profit：表示每种商品的利润，0<=profit[i]<=1000

其中：
1<=cost.length，profit.length<=200

输出描述

k 天后小明可以赚取的利润。

样例1
输入
3
2
2 5 7
2 5 7

输出
2

说明
小明初始有 2 元，仅能购买 0 号商品，第 1 天可赚取 2 元。
第 2 天小明有 4 元，但剩下的商品他都买不起了，所以他最终只能赚取 2 元。

样例2
输入
2
2
2 3 5
1 2 5

输出
3

说明
小明初始有 2 元，仅能购买 0 号商品，第 1 天可赚取 1 元。
第 2 天小明有 3 元，能购买 0 号和 1 号商品，选择购买 1 号商品，可赚取 2 元。
所以小明两天共赚取了 1+2=3 元。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  int k, m;
  cin >> k >> m;
  vector<int> cost;
  vector<int> profit;
  cin.ignore();
  string input;
  getline(cin, input);
  stringstream ss(input);
  int n;
  while (ss >> n) {
    cost.emplace_back(n);
  }
  string tmp2;
  getline(cin, tmp2);
  stringstream ss_2(tmp2);
  while (ss_2 >> n) {
    profit.emplace_back(n);
  }
  vector<pair<int,int>> goods;  
  for (int i = 0; i < cost.size(); ++i) {
    goods.push_back({cost[i], profit[i]});
  }
  sort(goods.begin(), goods.end());
  
  int curr_gold = m;
  priority_queue<int, vector<int>, less<int>> pq;
  int curr_good = 0;
  
  while (k--) {
    while (curr_good < goods.size() && curr_gold >= goods[curr_good].first) {
      pq.push(goods[curr_good].second);
      curr_good++;
    }
    if (pq.empty()) break;
    int today_profit = pq.top();
    pq.pop();
    curr_gold += today_profit;
  }

  cout << curr_gold - m << endl;
}