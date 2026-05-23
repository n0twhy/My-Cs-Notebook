/*
题目内容
小明正在开发浏览器地址栏功能，支持四种操作：visit（访问网页）、back（返回上一页）、forward（前进到下一页）、print（输出当前地址）。

初始状态
- 当前页面为 Blank，历史记录中只有 1 个 Blank 页面；
- 最多保存 max_history 个历史记录；
- 每次访问新页面时清空前进记录。

操作说明
visit url：
- 当前页面更新为该网页，加入历史记录；
- 若超过 max_history，则删除最早记录；
- 清空前进记录；
- 网址 url 为小写字母、数字和点的组合，长度 <= 100，用例数据均为合法输入。

back：
- 若历史记录至少有两个页面，切换到上一页，原当前页面加入前进记录；否则不做操作。

forward：
- 若前进记录不为空，切换到下一页，该页面加入历史记录；否则不做操作。

print：
- 输出当前页面地址，若为 Blank 则输出 Blank。

输入描述
第一行：整数 n（操作数，1 <= n <= 200）
第二行：整数 max_history（0 < max_history < 100）
接下来 n 行：操作命令。

输出描述
每次 print 操作输出当前地址，若无访问过任何页面则输出 Blank。

样例1
输入
7
10
visit a.com
visit b.com
back
visit c.com
print
forward
print

输出
c.com
c.com

说明
back 命令后，前进记录为 b.com；后续 visit 命令清空前进记录，因此 forward 命令无操作。

样例2
输入
7
3
visit a.com
visit b.com
visit c.com
visit d.com
back
forward
print

输出
d.com

说明
back 后，当前页面为 c.com，再 forward，当前页面为 d.com。

样例3
输入
9
3
visit a.com
visit b.com
visit c.com
visit d.com
visit e.com
back
back
back
print

输出
c.com

说明
容量为 3，历史记录中的页面为 c.com、d.com、e.com，两次 back 后，当前页面为 c.com，再次 back，前面再无页面，因此当前页面为 c.com。

样例4
输入
4
10
back
print
forward
print

输出
Blank
Blank

说明
初始页面为 Blank，历史记录中再无其他页面，因此 back 不做操作，forward 也不做操作，输出均为 Blank。

样例5
输入
4
10
visit abc.com
visit abc.com
back
print

输出
abc.com

说明
访问两次相同的页面场景，历史记录中为 Blank、abc.com、abc.com，因此 back 后，当前页面为 abc.com。
*/

#include <bits/stdc++.h>
#include <list>
#include <stack>
#include <utility>
#include <variant>
using namespace std;

list<string> history;
unordered_map<string, list<string>::iterator> mp;
stack<string> forward_st;
int max_history;

void visit(string &url) {
  while (!forward_st.empty()) forward_st.pop();
  history.push_back(url);
  if (history.size() > max_history) history.pop_front();

}

void forward(string &url) {
  if (forward_st.empty()) return;
  string fwd = forward_st.top();
  forward_st.pop();
  history.push_back(fwd);
}

void print(string &url) {
  cout << history.back() << endl;
}

void back(string &url) {
  if (history.size() < 2) return;
  string fwd = history.back();
  history.pop_back();
  forward_st.push(fwd);
}

int main() {
  int n;
  cin >> n >> max_history;
  history.push_back("Blank");
  cin.ignore();
  while (n--) {
    string op, url;
    string input;
    getline(cin, input);
    stringstream ss(input);

    ss >> op >> url;
    
    if (op == "visit") {
      visit(url);
    }
    if (op == "forward") {
      forward(url);
    }
    if (op == "back") {
      back(url);
    }
    if (op == "print") {
      print(url);
    }
  }
}