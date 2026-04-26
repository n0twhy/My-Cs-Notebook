/*
题目：大模型训练

题目描述
现有训练子任务模型列表 tasks，tasks[i] 表示第 i 个子任务的算力需求。
为了保证模型计算时效，要求所有任务在 T 个时刻内完成计算。

在任意一个时刻，可以调度多个子任务到量子计算机执行，
但该时刻内被调度任务的算力需求总和不能超过量子计算机的最大算力负荷。

请返回量子计算机所需要提供的最低算力，使得全部子任务可在 T 个时刻内完成。

输入描述
输入共两行：
第一行包含两个整数 N、T，分别表示子任务数量和时刻要求。
第二行包含 N 个整数：task[1] task[2] ... task[N]，
分别表示第 i 个子任务的算力需求。

输出描述
输出一行一个整数，表示量子计算机所需要的最低算力。

备注
1 <= T <= N <= 50000
1 <= tasks[i] <= 500

示例1
输入
10 5
1 2 3 4 5 6 7 8 9 10

输出
15

说明
最低算力为 15 时，可以在 5 个时刻内完成所有任务：
时刻1：完成 1,2,3,4,5
时刻2：完成 6,7
时刻3：完成 8
时刻4：完成 9
时刻5：完成 10

示例2
输入
6 3
4 4 2 1 2 3

输出
6

说明
时刻1：4
时刻2：4,2
时刻3：1,2,3
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int LargeModelTrain(int count, vector<int> &nums) {
    int sum = 0;
    for (auto i : nums) {
      sum += i;
    }
    int left = *(max_element(nums.begin(), nums.end()));
    int right = sum;
    int res = INT_MAX;

    while (left <= right) {
      int mid = (left + right) / 2;
      int curr = 0;
      int timestamp = 1;
      int i = 0;
      while (i < nums.size()) {
        if (curr + nums[i] > mid) {
          timestamp++;
          curr = 0;
          continue;
        }

        curr += nums[i];
        i++;
      }

      if (timestamp > count) {
        left = mid + 1;
      } else {
        res = min(res, mid);
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
  cout << sol.LargeModelTrain(m, nums) << endl;
}