/*
题目内容
给你 n 个字符串。我们称某个字符串 s_i 是“可删去的”，当且仅当存在两个下标 j, k(j != k)，满足 s_j + s_k = s_i。换句话说，我们称某个字符串是“可删去的”，当且仅当它能由两个来自原字符串序列中不同位置的非空字符串拼接而成。

你的任务是统计满足条件的字符串下标数量。

输入描述
每个测试文件均包含多组测试数据。第一行输入一个整数 T(1 <= T <= 2 * 10^5) 表示测试组数。每组测试数据的输入格式如下：

第一行输入一个整数 n，表示字符串数量；
此后 n 行，每行输入一个字符串。保证字符串长度至少为 1 且仅由小写字母组成。

除此之外，保证单个测试文件中所有测试组的 n 之和不超过 2 * 10^5，且全部字符串长度之和不超过 5 * 10^5。

输出描述
对于每一组测试数据，输出一行一个整数，表示该组中满足条件的字符串下标数量。

样例1
输入
2
5
a
b
ab
abc
bc
4
a
aa
a
aaa
输出
2
2

说明
对第一组：
"ab" 可以由 "a" + "b" 拼成；
"abc" 可以由 "a" + "bc" 拼成；
"a"、"b"、"bc" 本身不能由集合中两个非空字符串依次拼接得到；
因此答案为 2。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int CanDelete() {
    int n;
    cin >> n;
    vector<string> words(n);
    unordered_map<string, vector<int>> mp;
    for (int i = 0; i < n; ++i) {
      string input;
      cin >> input;
      words[i] = input;
      mp[input].emplace_back(i);
    }

    int res = 0;

    for (int i = 0; i < n; ++i) {
      int size = words[i].size();
      string curr = words[i];
      bool has_cut = false;
      for (int j = 0; j < size - 1; ++j) {
        if (has_cut) break;
        string left = curr.substr(0, j + 1);
        string right = curr.substr(j + 1);
        if (mp.count(left) && mp.count(right)) {
          if (left == right && mp[left].size() < 2) continue;
          res++;
          has_cut = true;
        }
      }
      
    }

    return res;
  }
};

int main() {
  int T;
  cin >> T;
  Solution sol;
  for (int i = 0; i < T; ++i) {
    cout << sol.CanDelete() << endl;
  }
}