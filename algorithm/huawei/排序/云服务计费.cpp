/*
题目描述
编写一个程序为基云服务计算客户话单。输入为某云服务的计费日志和各种计费因子的计费单价列表。
计费日志内容包含 4 个字段：时间戳、客户标识、计费因子、计费时长。

日志中如果同一客户、同一计费因子在相同时间戳上报多次话单，只能计费一次，选先上报的日志计费。
需要计算每个客户的话单总费用。

输入描述
第 1 行：表示计费日志的条数 n，为一个正整数，范围 1 ≤ n ≤ 1000。

第 2 到 n+1 行：表示云服务的计费日志，每行包含 4 列：
1) 时间戳：数字字符串，长度为 10；
2) 客户标识：字符串，长度为 1~16；
3) 计费因子：字符串，长度为 1~16（若计费因子在单价表中查不到，则认为该计费因子单价为 0）；
4) 计费时长：整数，范围为 0~100。

这 4 个字段使用英文逗号分隔。
当计费时长不在 [0,100] 范围内时，认为该计费日志有问题，将计费时长当成 0 处理。

第 n+2 行：表示计费因子的数量 m，为一个正整数，范围 1 ≤ m ≤ 100。

第 n+3 到 n+2+m 行：表示各种计费因子的计费单价列表，每行包含 2 列：
1) 计费因子：字符串，长度为 1~16；
2) 单价：正整数，范围为 1~100。

这 2 个字段使用英文逗号分隔。

输出描述
输出每个客户的话单总费用。
每行 2 列：第 1 列为客户名，第 2 列为话单费用，2 列用英文逗号分隔。
输出按客户标识字典序升序排序。

样例1
输入
5
1627845600,client1,factorA,10
1627845605,client2,factorB,15
1627845610,client1,factorA,5
1627845615,client1,factorB,8
1627845620,client2,factorB,20
2
factorA,5
factorB,7

输出
client1,131
client2,245

说明
client1 = 15×5 + 8×7 = 131
client2 = 0×5 + 35×7 = 245
*/
#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

int main() {
  int n; cin >> n;
  cin.ignore();
  vector<string> list;
  for (int i = 0; i < n; ++i) {
    string input;
    getline(cin, input);
    list.emplace_back(input);
  }
  map<string, int> price_factor;
  unordered_map<string, int> fee; 
  unordered_map<string, string> timestamps;

  int m; cin >> m;
  cin.ignore();
  while (m--) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    string factor;
    int single_price;
    getline(ss, factor, ',');
    ss >> single_price;
    price_factor[factor] = single_price;
  }

  for (int i = 0; i < n; ++i) {
    string timestamp;
    string client;
    string factor;
    int time;
    stringstream ss(list[i]);
    getline(ss, timestamp, ',');
    getline(ss, client, ',');
    getline(ss, factor, ',');
    ss >> time;
    if (!fee.count(client)) fee[client] = 0;
    if (timestamps[client] == timestamp) continue;
    if (time < 0 || time > 100) continue;
    if (!price_factor.count(factor)) continue;

    timestamps[client] = timestamp;
    fee[client] += time * price_factor[factor];
  }
  vector<string> res;
  for (auto [client, client_fee] : fee) {
    string tmp = client + "," + to_string(client_fee);
    res.emplace_back(tmp);
  }
  sort(res.begin(), res.end());
  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << endl;
  }
  
}
