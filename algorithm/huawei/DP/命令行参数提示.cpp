/*
题目内容
在使用命令行工具时，经常会出现手工输入错字母的情况。
例如原本想输入：
git clone
却输入成：
git clane

为了提升易用性，需要从支持的子命令列表中找出最相似的子命令进行提示。

“最相似”定义为最短莱文斯坦距离（Levenshtein Distance）：
即两个字符串之间，由一个转成另一个所需的最少编辑操作次数。

允许的编辑操作包括：
1. 将一个字符替换成另一个字符
2. 插入一个字符
3. 删除一个字符

输入描述
第一行：可提示的最短距离阈值 D
第二行：子命令数量 N
后面 N 行：所有子命令列表
最后一行：用户输入的子命令

参数范围：
1 <= D <= 5
1 <= N <= 30000
单个子命令长度 2 <= L <= 25，且仅包含小写字母

输出描述
输出分三种情况：

1. 用户输入的子命令与某个子命令完全匹配，输出该原命令。

2. 用户输入未完全匹配，但存在满足提示要求的子命令：
   输出所有满足条件的提示命令。
   若有多个，先按编辑距离从小到大排序；
   同一距离下按字母序从小到大排序。

3. 用户输入未匹配且不存在满足提示要求的子命令，输出 None。

样例1
输入
2
3
clone
checkout
switch
clane

输出
clone

说明
阈值为 2，表示当输入无法完全匹配时，
将编辑距离小于等于 2 的子命令作为提示输出。
本例中 clone 与 clane 的距离为 1，满足条件，因此输出 clone。

样例2
输入
2
3
clone
checkout
switch
create

输出
None

说明
没有子命令与 create 的编辑距离小于等于 2，因此输出 None。

样例3
输入
2
5
aprint
bprint
aaprint
bbprint
output
print

输出
aprint bprint aaprint bbprint

说明
与 print 距离小于等于 2 的有 4 个：
aprint、bprint（距离为 1）；
aaprint、bbprint（距离为 2）。
按“距离优先、同距离按字母序”输出结果。
*/

#include <bits/stdc++.h>
#include <string>
#include <utility>
using namespace std;

class Solution {
public:
  int CountDiffDegree(string &a, string &b) {
    int m = a.size();
    int n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 0; i <= m; ++i) {
      dp[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
      dp[0][j] = j;
    }

    for (int i = 1; i <= m; ++i) {
      for (int j = 1; j <= n; ++j) {

        if (a[i - 1] == b[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1];
        } else {
          dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
        }
      }
    }
    
    return dp[m][n];
  }
};

int main() {
  int d, n;
  cin >> d >> n;
  vector<string> commands(n);
  for (int i = 0; i < n; ++i) {
    cin >> commands[i];
  }
  string cmd;
  cin >> cmd;
  Solution sol;
  vector<pair<int, string>> res;
  for (auto &sub_command : commands) {
    int diff = sol.CountDiffDegree(cmd, sub_command);
    if (diff <= d) {
      res.emplace_back(diff, sub_command);
    }
  }
  if (res.empty()) {
    cout << "None" << endl;
    return 0;
  }

  sort(res.begin(), res.end(), [](const pair<int, string> &a, const pair<int, string> &b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second < b.second;
  });
  
  for (auto &[diff, sub_command] : res) {
    cout << sub_command << " ";
  }
  return 0;
}