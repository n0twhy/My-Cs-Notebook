/*
力扣 4. 寻找两个正序数组的中位数
https://leetcode.cn/problems/median-of-two-sorted-arrays/

题目描述：
给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的中位数。

要求：算法的时间复杂度应为 O(log(m + n))。

输入描述：
- 第一行：整数 m，表示数组 nums1 的长度。
- 第二行：m 个整数，表示 nums1（已按非递减排序；若 m 为 0 则该行可能为空，依平台约定）。
- 第三行：整数 n，表示数组 nums2 的长度。
- 第四行：n 个整数，表示 nums2（已按非递减排序；若 n 为 0 则该行可能为空，依平台约定）。

输出描述：
输出一行一个浮点数，表示两个数组合并后的中位数，精确到小数点后 5 位。

样例 1：
输入：
2
1 3
1
2
输出：
2.00000
说明：合并数组为 [1, 2, 3]，中位数为 2。

样例 2：
输入：
2
1 2
2
3 4
输出：
2.50000
说明：合并数组为 [1, 2, 3, 4]，中位数为 (2 + 3) / 2 = 2.5。

提示：
- nums1.length == m
- nums2.length == n
- 0 <= m <= 1000
- 0 <= n <= 1000
- 1 <= m + n <= 2000
- -10^6 <= nums1[i], nums2[i] <= 10^6
*/

#include <bits/stdc++.h>
#include <climits>
using namespace std;

class Solution {
public:
  double middlenum(vector<int> &num1, vector<int> &num2) {
    int size1 = num1.size();
    int size2 = num2.size();
    vector<int> longer = size1 > size2 ? num1 : num2;
    vector<int> shorter = size1 > size2 ? num2 : num1;
    shorter.push_back(INT_MAX);
    int mid = (size1 + size2 + 1) / 2;
    int i = 0;
    while (i >= 0 && i < shorter.size()) {
      int j = mid - i - 2;
      if ((shorter[i + 1] < longer[j]) || shorter[i] > longer[j + 1]) {
        i++;
        continue;
      }
      if ((size1 + size2) % 2) return max(shorter[i], longer[j]);
      int left = max(shorter[i], longer[j]);
      int right = min(shorter[i +1], longer[j + 1]);
      return (left + right) / (double)2;
    }
    return 0;
  }
};

int main() {
  int m;
  cin >> m;
  vector<int> nums1(m);
  for (int i = 0; i < m; ++i) {
    cin >> nums1[i];
  }
  int n;
  cin >> n;
  
  vector<int> nums2(n);
  for (int i = 0; i < n; ++i) {
    cin >> nums2[i];
    
  }
  
  Solution sol;
  cout << fixed << setprecision(5) << sol.middlenum(nums1, nums2) << endl;
}