/*
 * LeetCode 79 - 单词搜索 (Word Search)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一个 m×n 字符网格 board 和一个字符串 word，判断 word 是否存在于网格中。
 * 单词必须按字母顺序，通过相邻（上下左右）单元格的字母构成，同一单元格内的字母不允许被重复使用。
 *
 * 【输入格式】
 * 第一行：两个整数 m、n，表示网格行数和列数。
 * 接下来 m 行：每行 n 个字符（无空格），表示 board。字符均为大写字母。
 * 第 m+2 行：一个字符串 word，仅由大写字母组成。
 *
 * 【输出格式】
 * 一行：若存在输出 "true"，否则输出 "false"（无引号、小写）。
 *
 * 【样例输入 1】
 * 3 4
 * ABCE
 * SFCS
 * ADEE
 * ABCCED
 *
 * 【样例输出 1】
 * true
 *
 * 【样例输入 2】
 * 3 4
 * ABCE
 * SFCS
 * ADEE
 * SEE
 *
 * 【样例输出 2】
 * true
 *
 * 【样例输入 3】
 * 3 4
 * ABCE
 * SFCS
 * ADEE
 * ABCB
 *
 * 【样例输出 3】
 * false
 *
 * 【数据范围】
 * m、n 属于 [1, 6]，word 长度 [1, 15]
 * board 与 word 仅含大写英文字母
 *
 * =============================================================================
 */

 #include <exception>
#include <vector>
 #include <string>
 #include <iostream>
 using namespace std;

class Solution {
public:
  void helper(vector<vector<char>>& board, string& word, string& curr, int x, int y, int p) {
    if (res) return;
    if (curr == word) {
      res = true;
      return;
    }
    if(p >= word.size()) return;
    for(int i = 0; i < 4; ++i) {
      int cx = x + dx[i];
      int cy = y + dy[i]; 
      if (cx >= 0 && cx < m && cy >= 0 && cy < n && board[cx][cy] == word[p]) {
        char c = board[cx][cy];
        curr.push_back(board[cx][cy]);
        board[cx][cy] = '#';
        helper(board, word, curr, cx, cy, p + 1);
        curr.pop_back();
        board[cx][cy] = c;
      }
    }
  }
  bool search(vector<vector<char>>& board, string& word) {
    m = board.size();
    n = board[0].size();
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (board[i][j] == word[0]) {
          string curr;
          curr.push_back(word[0]);
          board[i][j] = '#';
          helper(board, word, curr, i, j, 1);
          curr.pop_back();
          board[i][j] = word[0];
          if(res) return true;
        }
      }
    }
    return res;
  }
private:
  bool res {false};
  int dx[4] = {-1, 1, 0, 0};
  int dy[4] = {0, 0, -1, 1};
  int m;
  int n;
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<vector<char>> board(m, vector<char>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> board[i][j];
    }
  }
  string word;
  cin >> word;
  Solution sol;
  bool res = sol.search(board, word);
  if (res) {
    cout << "true" << endl;
  } else {
    cout << "false" << endl;
  }
}