/*
 * LeetCode 1143 - 最长公共子序列 (Longest Common Subsequence)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定两个字符串 text1 和 text2，返回这两个字符串的最长公共子序列长度。
 * 如果不存在公共子序列，则返回 0。
 *
 * 定义：
 * 子序列：从原字符串中删除某些字符（可不删除），但不改变剩余字符的相对顺序，
 *         得到的新字符串。
 * 公共子序列：两个字符串都包含的子序列。
 *
 * 【输入格式】
 * 输入包含两行：
 * 第一行是字符串 text1（1 <= |text1| <= 1000）。
 * 第二行是字符串 text2（1 <= |text2| <= 1000）。
 * text1 和 text2 仅由小写英文字符组成。
 *
 * 【输出格式】
 * 一行一个整数，表示最长公共子序列的长度。
 *
 * 【样例输入 1】
 * abcde
 * ace
 *
 * 【样例输出 1】
 * 3
 *
 * （最长公共子序列是 "ace"，长度为 3）
 *
 * 【样例输入 2】
 * abc
 * abc
 *
 * 【样例输出 2】
 * 3
 *
 * （最长公共子序列是 "abc"，长度为 3）
 *
 * 【样例输入 3】
 * abc
 * def
 *
 * 【样例输出 3】
 * 0
 *
 * （两个字符串没有公共子序列，返回 0）
 *
 * 【数据范围】
 * 1 <= |text1| <= 1000
 * 1 <= |text2| <= 1000
 * text1 和 text2 仅由小写英文字符组成
 *
 * =============================================================================
 */

 #include <bits/stdc++.h>
 using namespace std;
 
class Solution {
public:
  int maxPublicSubstring(string& text1, string& text2) {
    int m = text1.size();
    int n = text2.size();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    int res = 0;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (text1[i] == text2[j]) {
          dp[i + 1][j + 1] = dp[i][j] + 1;
        } else {
          dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
        }
      }
    }

    return dp[m][n];
  }
};

int main() {
  string text1, text2;
  cin >> text1 >> text2;
  Solution sol;
  cout << sol.maxPublicSubstring(text1, text2) << endl;
}