/*
 * LeetCode 75 - 颜色分类 (Sort Colors)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个长度为 n 的数组 nums，其中包含红色、白色和蓝色三种颜色，
 * 分别用整数 0、1 和 2 表示。请对 nums 进行原地排序，使得相同颜色的元素相邻，
 * 并按红色、白色、蓝色顺序排列。
 *
 * 要求不使用内置排序函数。
 *
 * 【输入格式】
 * 输入包含两行：
 * 第一行输入一个整数 n（1 <= n <= 300），表示数组长度。
 * 第二行输入 n 个整数，表示数组 nums，其中 nums[i] ∈ {0, 1, 2}。
 *
 * 【输出格式】
 * 一行：排序后的数组，元素之间用空格分隔。
 *
 * 【样例输入 1】
 * 6
 * 2 0 2 1 1 0
 *
 * 【样例输出 1】
 * 0 0 1 1 2 2
 *
 * 【样例输入 2】
 * 3
 * 2 0 1
 *
 * 【样例输出 2】
 * 0 1 2
 *
 * 【数据范围】
 * 1 <= n <= 300
 * nums[i] ∈ {0, 1, 2}
 *
 * =============================================================================
 */

 #include <bits/stdc++.h>
#include <utility>
 using namespace std;

class Solution {
  public:
  vector<int> colorSort(vector<int> &colors) {
    int mid = 0;
    int low = 0;
    int high = colors.size() - 1;

    while (low <= high && mid <= high) {
      if (colors[mid] == 1) {
        mid++;
        continue;
      }

      if (colors[mid] == 0) {
        swap(colors[low], colors[mid]);
        low++;
        mid++;
        continue;
      }

      if (colors[mid] == 2) {
        swap(colors[high], colors[mid]);
        high--;
        continue;
      }
    }

    return colors;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> colors(n);
  
  for (int i = 0; i < n; ++i) {
    cin >> colors[i];
  }

  Solution sol;
  vector<int> res = sol.colorSort(colors);
  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
}