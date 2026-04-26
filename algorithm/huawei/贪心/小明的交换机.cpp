/*
题目内容

小明有 n 台交换机设备，用于搭建并行计算接入网络，给定长度为 n 的两个整数数组 port 和 bandwidth，
port[i] 代表第 i 台交换机的端口数量，bandwidth[i] 表示 i 台交换机单个端口的带宽（假设同一台交换机设备上各个端口的带宽相同）。
需要从这 n 台交换机中选择最多 k 台（可以小于 k）不同的交换机，使其组成的网络总带宽最大。
整个网络总带宽定义为所选交换机的总端口数量乘以所选交换机中端口带宽的最小值，
请你返回最多 k 台不同交换机的网络总带宽的最大值。

输入描述

第一行的输入是一个整数 n，表示交换机的数量，1≤n≤100。

第二行的输入是一个长度为 n 的整数数组 port，表示 n 台交换机的端口数量，
port[i] 表示第 i 台交换机的端口数量，20≤port[i]≤100。

第三行的输入是一个长度为 n 的整数数组 bandwidth，表示 n 台交换机的端口带宽，
bandwidth[i] 代表第 i 台交换机的端口带宽，10≤bandwidth[i]≤100。

第四行的输入是一个整数 k，表示最多选择的不同交换机数量，1≤k≤n。

输出描述

一个整数，即最多 k 台交换机的网络总带宽最大值。

样例1
输入
6
20 100 30 10 50 80
50 40 30 90 70 20
2

输出
6000

说明
从 6 台交换机设备中最多选出 2 台交换机，选择下标为 1（端口数量为 100，带宽为 40）、
下标为 4（端口数量为 50，带宽为 70）的两台交换机，则总带宽为最大：
(100+50)*min(40,70)=6000。

样例2
输入
5
100 20 50 50 80
100 10 20 20 20
3

输出
10000

说明
此时我们选择下标为 0 的交换机，得到最大的总带宽为：
100*100=10000。
*/

#include <bits/stdc++.h>
#include <queue>
#include <vector>
using namespace std;

struct Switch {
  int port;
  int bandwidth;
};

int main() {
  int n;
  cin >> n;
  vector<Switch> switchs(n);
  for (int i = 0; i < n; ++i) {
    cin >> switchs[i].port;
  }
  for (int i = 0; i < n; ++i) {
    cin >> switchs[i].bandwidth;
  }
  int k;
  cin >> k;

  sort(switchs.begin(), switchs.end(), [](const Switch &a, const Switch &b) {
    return a.bandwidth > b.bandwidth;
  });

  priority_queue<int, vector<int>, greater<int>> pq;
  int res = 0;
  int port_sum = 0;
  for (int i = 0; i < switchs.size(); ++i) {
    pq.push(switchs[i].port);
    port_sum += switchs[i].port;
    if (pq.size() > k) {
      port_sum -= pq.top();
      pq.pop();
    }
    res = max(res, port_sum * switchs[i].bandwidth);
  }

  cout << res << endl;
}