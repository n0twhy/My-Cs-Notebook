/*
题目：K统治对的计数

题目内容
给定一个长度为 n 的整数数组 A，以及一个整数 K。
定义下标对 (i,j)（满足 i<j），如果 A[i]-A[j] >= K，则称其为一个「K-统治对」。

请统计并返回数组中所有「K-统治对」的数量。

输入描述
第一行：数组长度 n，范围 [1,100000]
第二行：数组元素（空格分隔），元素范围 [-100000,100000]
第三行：整数 K

输出描述
输出一个整数，表示数组中所有「K-统治对」的数量。

样例1
输入
5
-3 -1 -6 2 0
2

输出
3

说明
满足 A[i]-A[j]>=2 且 i<j 的下标对有：
(0,2): -3-(-6)=3
(1,2): -1-(-6)=5
(3,4): 2-0=2
共 3 个。

样例2
输入
5
2 9 4 7 1
2

输出
5

说明
满足 A[i]-A[j]>=2 且 i<j 的下标对有：
(1,2): 9-4=5
(1,3): 9-7=2
(1,4): 9-1=8
(2,4): 4-1=3
(3,4): 7-1=6
共 5 个。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

int lowbit(int x) {
  return x & (-x);
}

void update(vector<int> &tree, int curr, int add) {
  while (curr < tree.size()) {
    tree[curr] += add;
    curr += lowbit(curr);
  }
}

int query(vector<int> &tree, int curr) {
  int sum = 0;
  while (curr > 0) {
    sum += tree[curr];
    curr -= lowbit(curr); 
  }
  return sum;
}

class Solution {
public:
  int CountK(vector<int> &nums, int k) {
    int n = nums.size();
    vector<int> tree(200002 + k, 0);
    
    for (int i = 0; i < nums.size(); ++i) {
      nums[i] += 100001;
    }

    int res = 0;

    for (auto num : nums) {
      int search = k + num - 1;
      res += query(tree, 200001 + k) - query(tree, search);
      update(tree, num, 1);
    }
    return res;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> nums(n);
  for (int i = 0; i < n; ++i) {
    cin >> nums[i];
  }
  int k;
  cin >> k;
  Solution sol;
  cout << sol.CountK(nums, k);
}