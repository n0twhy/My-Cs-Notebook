/*
力扣 79. 单词搜索
https://leetcode.cn/problems/word-search/

题目描述：
给定一个 m x n 二维字符网格 board 和一个字符串 word。如果 word 存在于网格中，返回 true；否则返回 false。

单词必须按照字母顺序，通过相邻（水平或垂直）单元格内的字母构成。同一个单元格内的字母不允许被重复使用。

输入描述：
- 第一行：两个整数 m、n，表示网格行数和列数。
- 接下来 m 行：每行 n 个字符（可按空格分隔输入），表示 board。
- 最后一行：字符串 word。

输出描述：
输出一行：若存在则输出 true，否则输出 false（小写）。

样例 1：
输入：
3 4
A B C E
S F C S
A D E E
ABCCED
输出：
true

样例 2：
输入：
3 4
A B C E
S F C S
A D E E
SEE
输出：
true

样例 3：
输入：
3 4
A B C E
S F C S
A D E E
ABCB
输出：
false

提示：
- m == board.length
- n = board[i].length
- 1 <= m, n <= 6
- 1 <= word.length <= 15
- board 和 word 仅由大小写英文字母组成
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int dx[4] = {-1, 1, 0,0};
  int dy[4] = {0, 0, -1, 1};
  
  void dfs(vector<vector<char>> &grid, string &key, int x, int y, int curr_pos) {
    if (curr_pos == key.size()) search = true;

    for (int i = 0; i < 4; ++i) {
      int cx = x + dx[i];
      int cy = y + dy[i];
      if (cx >= 0 && cx < grid.size() && cy >= 0 && cy < grid[0].size() && grid[cx][cy] == key[curr_pos]) {
        grid[cx][cy] = '#';
        dfs(grid, key, cx, cy, curr_pos + 1);
        grid[cx][cy] = key[curr_pos];
      }
    }
  }
  
  bool WordSearch(vector<vector<char>> &grid, string &key) {
    for (int i = 0; i < grid.size(); ++i) {
      if (search) return search;
      for (int j = 0; j < grid[i].size(); ++j) {
        if (grid[i][j] == key[0]) {
          dfs(grid, key, i , j, 1);
        }
      }
    }

    return search;
  }
private:
  bool search {false};
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<vector<char>> grid(m, vector<char>(n));

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> grid[i][j];
    }
  }

  string key;
  cin >> key;
  Solution sol;
  bool res = sol.WordSearch(grid, key);
  if (res) {
    cout << "ture" << endl;
  } else {
    cout << "false" << endl;
  }
}