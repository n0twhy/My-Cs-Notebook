/*
题目内容
给你两个数组，请你返回这两个数组中“长度相同的非空子数组”的最大绝对差和。

两个长度相同数组的绝对差和定义为：
对应位置元素差值的绝对值之和。
例如：
数组1为 [1, 2]，数组2为 [3, 4]，
绝对差和为 |1-3| + |2-4| = 2 + 2 = 4。

这里的“子数组”按题面定义为：
通过删除原数组中的某些元素（可不删）后，保持相对顺序得到的新数组
（即更接近“子序列”的定义）。
例如：[2, 3, 5] 是 [1, 2, 3, 4, 5] 的一个子数组；
[1, 5, 3] 不是，因为相对顺序不一致。

输入描述
第1行：
N M
N 表示数组1的长度，M 表示数组2的长度。
范围：1 <= N, M <= 500

第2行：
数组1中的 N 个整数，数值范围 [-1000, 100]

第3行：
数组2中的 M 个整数，数值范围 [-1000, 100]

输出描述
输出一个整数，表示长度相同的非空子数组最大绝对差和。

样例1
输入
3 3
1 3 5
2 4 6

输出
6

说明
可取数组1的子数组 [1, 3]，数组2的子数组 [4, 6]，
绝对差和为 |1-4| + |3-6| = 6，为最大值。

样例2
输入
2 2
1 2
3 4

输出
4

说明
可取数组1的子数组 [1, 2]，数组2的子数组 [3, 4]，
绝对差和为 |1-3| + |2-4| = 4，为最大值。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int MaxDiffSum(vector<int>& a, vector<int> &b) {
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
      for (int j = 1; j <= n; ++j) {
        dp[i][j] = max({abs(a[i- 1] - b[j - 1]) + dp[i - 1][j - 1], dp[i][j - 1], dp[i - 1][j]});
      }
    }
    
    return dp[m][n];
  }
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<int> a(m);
  vector<int> b(n);
  for (int i = 0; i < m; ++i) {
    cin >> a[i];
  }
  for (int i = 0; i < n; ++i) {
    cin >> b[i];
  }
  Solution sol;
  cout << sol.MaxDiffSum(a, b) << endl;
}