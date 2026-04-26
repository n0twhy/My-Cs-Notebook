/*
力扣 139. 单词拆分
https://leetcode.cn/problems/word-break/

题目描述：
给你一个字符串 s 和一个字符串列表 wordDict 作为字典。如果可以利用字典中出现的一个或多个单词拼接出 s，则返回 true。

注意：不要求字典中的单词全部使用；字典中的单词可以重复使用。

输入描述：
- 第一行：字符串 s。
- 第二行：若干个单词，空格分隔，依次为 wordDict 中的每一项（本题中每个单词仅由小写字母组成、不含空格）。

输出描述：
输出一行：若能用字典中的单词（可重复、不必用完）拼接出整个 s，输出 true；否则输出 false。

样例 1：
输入：
leetcode
leet code
输出：
true
说明：leetcode 可以由 leet 和 code 拼接而成。

样例 2：
输入：
applepenapple
apple pen
输出：
true
说明：applepenapple 可以由 apple、pen、apple 拼接而成，单词可重复使用。

样例 3：
输入：
catsandog
cats dog sand and cat
输出：
false

提示：
- 1 <= s.length <= 300
- 1 <= wordDict.length <= 1000
- 1 <= wordDict[i].length <= 20
- s 和 wordDict[i] 仅由小写英文字母组成
- wordDict 中的所有字符串互不相同
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  bool CanSparse(string &s, vector<string> &words) {
    unordered_set<string> mp;
    for (auto &word : words) {
      mp.insert(word);
    }
    vector<bool> dp(s.size() + 1, false);
    dp[0] = true;
    for (int i = 1; i <= s.size(); ++i) {
      for (int j = 0; j < i; ++j) {
        if (dp[j] && mp.count(s.substr(j, i - j))) {
          dp[i] = true;
          break;
        }
      }
    }
    return dp[s.size()];
  }
};

int main() {
  string s;
  cin >> s;
  cin.ignore();
  string input;
  getline(cin, input);
  stringstream ss(input);
  vector<string> words;
  string word;
  while (ss >> word) {
    words.emplace_back(word);
  }
  Solution sol;
  bool res = sol.CanSparse(s, words);
  if (res) {
    cout << "true" << endl;
  } else {
    cout << "false" << endl;
  }
}