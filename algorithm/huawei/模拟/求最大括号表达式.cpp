/*
题目：求最大括号表达式

题目内容
有效括号表达式定义：
1. 空串和 "()" 均为有效表达式。
2. 当 A、B 为有效表达式时，则 "(A)"、"AB" 也均是有效括号表达式。
   例如：A 为 "()"，则 "()()" 和 "(())" 均为有效括号表达式。

括号表达式的值：
左括号用 1 表示，右括号用 0 表示，该二进制序列对应的值即为括号表达式的值。

现给定一个有效括号表达式，对其中任意两个相邻的子“有效表达式”进行交换，
求在任意次数（包含 0 次）交换之后，能够得到的值最大的括号表达式。

说明：
1. 表达式自身是有效表达式。
2. 交换的必须是相邻且有效的子表达式。

输入描述
给定一个有效括号表达式，只包含左右括号 "()"。
表达式长度不超过 60。

输出描述
输出在任意次数（包含 0 次）交换后，能够得到值最大的括号表达式。

样例1
输入：
((()(())))

输出：
(((())()))

说明：
将 s[2] 处出现的有效表达式 "()" 和 s[4] 处出现的有效表达式 "(())" 进行交换。
对应二进制：11 10 1100 00 转换为 11 1100 10 00。

样例2
输入：
()()

输出：
()()

说明：
无需交换，交换后也一样。

样例3
输入：
()(())((()(())))

输出：
(((())()))(())()

说明（交换过程示例）：
1) 交换 s[8-9] 与 s[10-13]，得到：()(())(((())()))
2) 交换 s[2-5] 与 s[6-15]，得到：()(((())()))(())
3) 交换 s[0-1] 与 s[2-11]，得到：(((())()))()(())
4) 交换 s[10-11] 与 s[12-15]，得到：(((())()))(())()
*/

#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  struct TreeNode {
    vector<TreeNode *> children;
  };
  
  string helper(TreeNode* node, bool is_root) {
    if (node->children.size() == 0) return "10";
    vector<string> childsrts;
    for (auto child : node->children) {
      childsrts.push_back(helper(child, false));
    }
    sort(childsrts.begin(), childsrts.end(), [](const string &a, const string &b){
      return a + b > b + a;
    });

    string mid = "";
    for (auto s : childsrts) {
      mid += s;
    }

    if (is_root) return mid;
    return "1" + mid + "0";
  }
  
  string buildTree(string& s) {
    stack<TreeNode *> st;
    st.push(new TreeNode());
    
    for (auto& c : s) {
      if (c == '(') {
        st.push(new TreeNode);
      }
      if (c == ')') {
        auto node = st.top();
        st.pop();
        st.top()->children.push_back(node);
      }
    }

    string tmp_res = helper(st.top(), true);
    string res;
    for (auto& c : tmp_res) {
      if (c == '1') {
        res += '(';
      } else {
        res += ')';
      }
    }

    return res;
  }
  
};

int main() {
    string input;
    cin >> input;

    Solution sol;
    cout << sol.buildTree(input) << endl;
}
