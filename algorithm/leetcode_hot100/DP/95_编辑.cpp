/*
 * LeetCode 72 - 编辑距离 (Edit Distance)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定两个单词 word1 和 word2，请返回将 word1 转换为 word2 所需的最少操作数。
 *
 * 允许的操作如下：
 * - 插入一个字符
 * - 删除一个字符
 * - 替换一个字符
 *
 * 【输入格式】
 * 输入包含两行：
 * 第一行是字符串 word1（0 <= |word1| <= 500）。
 * 第二行是字符串 word2（0 <= |word2| <= 500）。
 * word1 和 word2 仅由小写英文字母组成。
 *
 * 【输出格式】
 * 一行一个整数，表示将 word1 转换为 word2 所需的最少操作数。
 *
 * 【样例输入 1】
 * horse
 * ros
 *
 * 【样例输出 1】
 * 3
 *
 * （说明）
 * horse -> rorse （替换 h -> r）
 * rorse -> rose  （删除 r）
 * rose  -> ros   （删除 e）
 *
 * 【样例输入 2】
 * intention
 * execution
 *
 * 【样例输出 2】
 * 5
 *
 * （说明）
 * intention -> inention  （删除 t）
 * inention  -> enention  （替换 i -> e）
 * enention  -> exention  （替换 n -> x）
 * exention  -> exection  （替换 n -> c）
 * exection  -> execution （插入 u）
 *
 * 【数据范围】
 * 0 <= |word1| <= 500
 * 0 <= |word2| <= 500
 * word1 和 word2 仅由小写英文字母组成
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int Edition(string& word1, string& word2) {
    int m = word1.size();
    int n = word2.size();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));

    for (int i = 0; i < m; ++i) {
      dp[i][0] = i;
    }

    for (int i = 0; i < n; ++i) {
      dp[0][i] = i;
    }

    for (int i = 1; i <= m; ++i) {
      for (int j = 1; j <= n; ++j) {
        if (word1[i - 1] == word2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1];
        } else {
          dp[i][j] = min({dp[i - 1][j], dp[i - 1][j - 1], dp[i][j - 1]}) + 1;
        }
      }
    }

    return dp[m][n];
  }
};

int main() {
  string word1;
  string word2;
  cin >> word1 >> word2;
  Solution sol;
  cout << sol.Edition(word1, word2) << endl;
}