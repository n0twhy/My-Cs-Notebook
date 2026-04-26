/*
题目内容
给定一个仅由字符 '0' 与 '1' 组成、长度为 n 的环形二进制串 s。
你可以选择一个起点，将环断开并从该位置起按顺时针读出，得到一个线性串（等价于对 s 进行一次旋转）。在得到的线性串中，定义其最短 k-前缀长度为：包含恰好 k 个字符 '1' 的最短前缀的长度（若整个线性串中 '1' 的总数小于 k，则输出 -1）。

你的任务是：在所有可能的旋转中，取上述最短 k-前缀长度的最小值；若对于任意旋转均不存在包含 k 个 '1' 的前缀，则输出 -1。

【名词解释】
字符串的前缀：从字符串的第一个字符开始，向后连续取若干个字符得到的字符串。更具体地，字符串 s 前 i 个字符构成的字符串被称为 s 的第 i 个前缀，记为 s[1..i]。

输入描述
每个测试文件包含多组测试数据。第一行输入一个整数 T(1 <= T <= 10^5) 表示数据组数，每组测试数据描述如下：

每组输入第一行包含两个整数 n, k(1 <= n <= 2 * 10^5, 1 <= k <= n)。
第二行输入一个长度为 n 的仅由字符 '0' 与 '1' 构成的字符串 s。
保证所有测试数据的 n 之和不超过 2 * 10^5。

输出描述
对于每组测试数据，输出一行一个整数，表示在所有旋转中包含恰好 k 个 '1' 的最短前缀长度的最小值；若不存在，输出 -1。

样例1
输入
3
8 3
11001001
5 2
01000
7 1
0000001
输出
3
-1
1
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int MinimalPre() {
    int n, k;
    cin >> n >> k;
    cin.ignore();
    string s;
    getline(cin, s);

    for (int i = 0; i < n; ++i) {
      s.push_back(s[i]);
    }

    vector<int> ones;
    for (int i = 0; i < s.size(); ++i) {
      if (s[i] == '1') ones.emplace_back(i);
    }

    if (ones.size() / 2 < k) return -1;

    int left = 0;
    int right = k - 1;

    int res = ones[right] - ones[left] + 1;

    while (right < ones.size()) {
      if (ones[right] - ones[left] + 1 <= n) {
        res = min(res, ones[right] - ones[left] + 1);
      }
      right++;
      left++;
    }
    return res;
  }
};

int main() {
  int T;
  cin >> T;
  Solution sol;
  for (int i = 0; i < T; ++i) {
    cout << sol.MinimalPre() << endl;
  }
}