/*
题目：坐火车

题目内容
某旅行线路上有一辆列车，共 M 节车厢，初始所有车厢都无人。
现有 N 个旅行团需要搭乘该线路，旅行团 ID 为 [0, N)。

请给出乘坐方案，使各车厢间乘客数量差异最小。
设乘客最多的车厢人数为 X，最少的车厢人数为 Y，需要使 X-Y 最小。

旅行团乘坐需满足：
1、对于任意 0 < i < j < M，车厢 i 中任意旅行团的 ID 小于车厢 j 中任意旅行团的 ID。
2、同一车厢内的旅行团，其 ID 必须连续。
3、同一旅行团成员必须全部在同一节车厢内。

注：当只有一节车厢时，该车厢人数既是最大值也是最小值。

输入描述
第 1 行：N M
- N 为旅行团个数，范围 [1,1000]
- M 为车厢个数，范围 [1,N]

第 2 行：P1 P2 ... PN
其中 Pi 表示第 i 个旅行团成员个数，范围 [1,100000]。

输出描述
输出乘客最多的车厢中的人数 X。

样例1
输入
3 3
1 6 4

输出
6

说明
有 3 个旅行团、3 节车厢，成员数分别为 1、6、4。
每节车厢各放一个旅行团时差异最小，方案为 [1][6][4]，
乘客最多车厢人数为 6。

样例2
输入
5 2
7 2 5 10 8

输出
18

说明
有 5 个旅行团、2 节车厢，成员数分别为 7、2、5、10、8。
可行划分包括：
[7][2 5 10 8] -> 7 和 25，差异 18
[7 2][5 10 8] -> 9 和 23，差异 14
[7 2 5][10 8] -> 14 和 18，差异 4
[7 2 5 10][8] -> 24 和 8，差异 16

最佳方案是 [7 2 5][10 8]，其中乘客最多车厢人数为 18。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int Train(int cnt, vector<int> &nums) {
    auto it_max = max_element(nums.begin(), nums.end());
    int sum = 0;
    for (auto i : nums) {
      sum += i;
    }

    int left = *it_max;
    int right = sum;
    int res = INT_MAX;

    while (left <= right) {
      int mid = (left + right) / 2;
      int curr = 0;
      int car_cnt = 1;
      int curr_max = 0;
      int i = 0;
      while (i < nums.size()) {
        if (curr + nums[i] > mid) {
          curr_max = max(curr_max, curr);
          curr = 0;
          car_cnt++;
          continue;
        }
        curr += nums[i];
        if (i == nums.size() - 1) {
          curr_max = max(curr_max, curr);
        }
        i++;
      }

      if (car_cnt > cnt) {
        left = mid + 1;
      } else {
        res = min(res, curr_max);
        right = mid - 1;
      }
    }
    return res;
  }
};

int main() {
  int n, m;
  cin >> n >> m;
  vector<int> nums(n);

  for (int i = 0; i < n; ++i) {
    cin >> nums[i];
  }

  Solution sol;
  cout << sol.Train(m, nums) << endl;
}