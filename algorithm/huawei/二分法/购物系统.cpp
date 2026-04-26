/*
题目：购物系统

题目内容
在一个购物 APP 中，有一个核心购物系统，它的接口被 N 个客户端调用。
每个客户端在一定时间内的调用量为 R=[R1,R2,...,RN]。

由于核心购物系统出现集群故障，必须临时降级并限制调用量。
核心购物系统最多可接受的调用总量为 cnt。

降级规则如下：
1、如果 sum(R1,R2,...,RN) <= cnt，则全部客户端可正常调用，返回 -1。
2、如果 sum(R1,R2,...,RN) > cnt，则需要设定阈值 value（value 可为 0）：
   - 当某客户端调用量 Ri > value 时，将其调用量限制为 value；
   - 当 Ri <= value 时，保持 Ri 不变。

要求在“限制后总调用量不超过 cnt”的前提下，求最大的 value。

输入描述
第一行：客户端调用量数组（整型数组，以空格分隔）。
第二行：核心购物系统的最大调用量 cnt。

参数范围：
0 < R.length <= 10^5
0 <= R[i] <= 10^5
0 <= cnt <= 10^9

输出描述
输出调用量阈值 value。

样例一
输入
1 4 2 5 5 1 6
13

输出
2

样例解释
因为 1+4+2+5+5+1+6 > 13，
当 value=2 时，限制后调用量为 1+2+2+2+2+1+2=12 < 13，
因此 value 为 2。

样例二
输入
1 7 8 8 1 0 2 4 9
7

输出
0

样例解释
即使 value=1，限制后调用量仍然大于 7，
因此 value 只能取 0。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <cstdio>
#include <vector>
using namespace std;


class Solution {
public:
  long long ShoppingSystem(vector<long long> &nums, long long cnt) {
    long long n = nums.size();
    
    sort(nums.begin(), nums.end());
    long long left = 0;
    long long right = nums[n - 1];

    vector<long long> pre_sum;
    long long sum = 0;

    for (auto i : nums) {
      sum += i;
      pre_sum.emplace_back(sum);
    }

    if (sum <= cnt) return -1;

    long long value = -1;

    while (left <= right) {
      long long mid = (left + right) / 2;
      auto it = upper_bound(nums.begin(), nums.end(), mid);
      long long pos = it - nums.begin();
      long long curr_sum = 0;

      if (pos == 0) {
        curr_sum = n * mid;
      } else {
        curr_sum = pre_sum[pos - 1] + (n - pos) * mid;
      }

      if (curr_sum > cnt) {
        right = mid - 1;
      }  else {
        value = mid;
        left = mid + 1;
      }
    }

    return value;
  }
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  long long n;
  vector<long long> nums;
  while (ss >> n) {
    nums.emplace_back(n);
  }

  long long cnt;
  cin >> cnt;
  Solution sol;
  cout << sol.ShoppingSystem(nums, cnt) << endl;
}