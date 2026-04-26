/*
力扣 131. 分割回文串
https://leetcode.cn/problems/palindrome-partitioning/

题目描述：
给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是回文串。返回 s 所有可能的分割方案。

输入描述：
一行，一个仅由小写英文字母组成的字符串 s。

输出描述：
若干行，每一行表示一种分割方案：各段回文子串之间用一个空格分隔（顺序可与题解一致，一般按 DFS 枚举顺序输出）。

样例 1：
输入：
aab
输出：
a a b
aa b

样例 2：
输入：
a
输出：
a

提示：
- 1 <= s.length <= 16
- s 仅由小写英文字母组成
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  void dfs(string &s, int start) {
    if (start == s.size()) {
      ans.emplace_back(curr);
    }

    for (int end = start; end < s.size(); ++end) {
      if (dp[start][end]) {
        curr.emplace_back(s.substr(start, end - start + 1));
        dfs(s, end + 1);
        curr.pop_back();        
      }
    }
  }

  vector<vector<string>> partition(string s) {
    dp.resize(s.size(), vector<bool>(s.size(), false));
    
    for (int i = s.size() - 1; i >= 0; --i) {
      for(int j = i; j < s.size(); ++j) {
        if (s[i] == s[j] && (j - i < 2 || dp[i + 1][j - 1])) {
          dp[i][j] = true;
        }
      }
    }



    dfs(s, 0);
    return ans;

  }
private:
  vector<vector<bool>> dp;
  vector<vector<string>> ans;
  vector<string> curr;
};

int main() {
  string input;
  cin >> input;
  Solution sol;
  vector<vector<string>> ans = sol.partition(input);
  for (int i = 0; i < ans.size(); ++i) {
    for (int j = 0; j < ans[i].size(); ++j) {
      cout << ans[i][j] << " ";
    }
    cout << endl;
  }
}