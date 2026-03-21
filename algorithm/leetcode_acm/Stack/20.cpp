/*
 * LeetCode 20 - 有效的括号 (Valid Parentheses)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s，判断字符串是否有效。
 * 有效需满足：左括号必须用相同类型的右括号闭合，且左括号必须以正确的顺序闭合。
 *
 * 【输入格式】
 * 一行，一个字符串 s，仅包含上述六种字符，长度至少为 1。
 *
 * 【输出格式】
 * 一行：若有效输出 "true"，否则输出 "false"（无引号、小写）。
 *
 * 【样例输入 1】
 * ()
 *
 * 【样例输出 1】
 * true
 *
 * 【样例输入 2】
 * ()[]{}
 *
 * 【样例输出 2】
 * true
 *
 * 【样例输入 3】
 * (]
 *
 * 【样例输出 3】
 * false
 *
 * 【数据范围】
 * 1 <= s.length <= 10^4
 * s 仅由 '()[]{}' 组成
 *
 * =============================================================================
 */
#include <stack>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Solution {
public:
  bool valid(string& s) {
    stack<char> st;
    for (char& c : s) {
      if (c == '(' || c == '[' || c == '{') {
        st.push(c);
      }

      if (c == ')' || c ==']' || c =='}') {
        if (st.empty()) return false;
        char tmp = st.top();
        st.pop();
        if ((c == ')' && tmp != '(') || (c == ']' && tmp != '[') || (c == '{' && tmp != '{')) {
          return false;
        }
      }
    }
    if (!st.empty()) return false;
    return true;
  }
  
};

int main() {
  string s;
  cin >> s;
  Solution sol;
  bool res = sol.valid(s);
  if (res) {
    cout << "true" << endl;
  } else {
    cout << "false" << endl;
  }
}