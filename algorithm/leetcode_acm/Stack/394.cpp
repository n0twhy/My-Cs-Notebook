/*
 * LeetCode 394 - 字符串解码 (Decode String)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个经过编码的字符串，返回它解码后的字符串。
 * 编码规则：k[encoded_string]，表示方括号内的 encoded_string 重复 k 次。k 为正整数。
 * 保证输入字符串有效；方括号内可能包含字母和嵌套的 k[encoded_string]。
 *
 * 【输入格式】
 * 一行，编码字符串 s，仅包含小写字母、数字和方括号 '[' ']'。
 *
 * 【输出格式】
 * 一行，解码后的字符串（不包含数字和方括号）。
 *
 * 【样例输入 1】
 * 3[a]2[bc]
 *
 * 【样例输出 1】
 * aaabcbc
 *
 * 【样例输入 2】
 * 3[a2[c]]
 *
 * 【样例输出 2】
 * accaccacc
 *
 * 【样例输入 3】
 * 2[abc]3[cd]ef
 *
 * 【样例输出 3】
 * abcabccdcdcdef
 *
 * 【数据范围】
 * 1 <= s.length <= 30
 * s 由小写英文字母、数字和方括号组成
 * s 保证为合法编码串；数字仅表示重复次数，在 [1, 300] 内
 *
 * =============================================================================
 */

#include <cctype>
#include <stack>
#include <string>
#include <iostream>
using namespace std;
class Solution {
public:
  string decode(string& s) {
    stack<string> str;
    stack<int> digit;
    string curr = "";
    int currentK = 0;;
    for (auto& c : s) {
      if (isdigit(c)) {
        currentK = 10 * currentK + c - '0';
      } else if (c == '[') {
        digit.push(currentK);
        currentK = 0;
        str.push(curr);
        curr = "";
      } else if (c == ']') {
        int k = digit.top();
        digit.pop();
        string pre = str.top();
        str.pop();
        for (int i = 0; i < k; ++i) {
          pre += curr;
        }
        curr = pre;
      } else {
        curr += c;
      }
    }
    return curr;
  }
};

int main() {
  string s;
  cin >> s;
  Solution sol;
  string res = sol.decode(s);
  cout << res << endl;
}

