/*
题目内容

两队学生拿着号码牌，排成 1 队，选择队伍中连续的几个学生将他们按照号码牌就地升序排序之后连接起来，
使得连接的结果和直接整体按升序排列后的结果一致。

输入描述

一组正整数数组。

如：
5 5 4 4 3 3 2 2 1 1，数组长度为【1,500】。

输出描述

最多将学生分成多少块。

样例1
输入
2 1 3 4 4 5

输出
5

说明
分成 [2,1],[3],[4],[4],[5] 可以得到最多的块数。

样例2
输入
5 4 3 2 1

输出
1

说明
将数组分成 2 块或者更多块，都无法得到所需的结果。

例如，分成 [5,4],[3,2,1] 的结果是 [4,5,1,2,3]，这不是有序的数组。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  vector<int> nums;
  int element;
  while (ss >> element) {
    nums.emplace_back(element);
  }
  int n = nums.size();
  vector<int> pre_max(n);
  vector<int> post_min(n);
  int curr_max = nums[0];
  for (int i = 0; i < n; ++i) {
    curr_max = max(curr_max, nums[i]);
    pre_max[i] = curr_max;
  }
  int curr_min = nums[n - 1];
  for (int i = n - 1; i >= 0; --i) {
    curr_min = min(curr_min, nums[i]);
    post_min[i] = curr_min;
  }
  int res = 0;
  for (int i = 0; i < n - 1; ++i) {
    if (pre_max[i] <= post_min[i + 1]) res++;
  }
  cout << res + 1 << endl;
}