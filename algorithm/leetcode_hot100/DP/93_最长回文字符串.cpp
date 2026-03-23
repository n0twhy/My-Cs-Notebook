/*
 * LeetCode 5 - 最长回文子串 (Longest Palindromic Substring)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个字符串 s，找到 s 中最长的回文子串。
 *
 * 【输入格式】
 * 一行：一个字符串 s（1 <= |s| <= 1000），仅由数字和英文字母组成。
 *
 * 【输出格式】
 * 一行：表示 s 中最长的回文子串。
 * 若存在多个答案，返回任意一个。
 *
 * 【样例输入 1】
 * babad
 *
 * 【样例输出 1】
 * bab
 *
 * （aba 也是符合题意的答案）
 *
 * 【样例输入 2】
 * cbbd
 *
 * 【样例输出 2】
 * bb
 *
 * 【数据范围】
 * 1 <= |s| <= 1000
 * s 仅由数字和英文字母组成
 * s 至少包含 1 个字符
 * 若存在多个答案，返回任意一个
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  string longest(string& s) {
    int n = s.size();
    int start = 0;
    int maxlength = 1;
    vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), false));
    
    for (int i = 0; i < s.size(); ++i) {
      dp[i][i] = true;
    }
    
    for (int length = 2; length <= n; ++length) {
      for (int i = 0; i <= n - length; ++i) {
        int j = i + length - 1;
        if (s[i] == s[j]) {
          if (length == 2 || dp[i + 1][j - 1]) {
            dp[i][j] = true;
            if (length > maxlength) {
              start = i;
              maxlength = length;
            }
          }
        }
      }
    }

    return s.substr(start, maxlength);
  }
};

int main() {
  string input;
  cin >> input;
  Solution sol;
  cout << sol.longest(input) << endl;
  
}