/*
题目：小美的因子数量

题目内容
小美很喜欢因子数量为奇数的数。现在小芳给了小美一个区间 [l, r]，请你帮小美算出区间内有多少个因子数量为奇数的数。

【名词解释】
因子：对于正整数 a，如果存在正整数 p 使得 a 能被 p 整除，则称 p 是 a 的因子。
例如，12 的因子有 1, 2, 3, 4, 6, 12。

输入描述
第一行输入两个整数 l, r（1 <= l <= r <= 10^9），表示询问的区间。

输出描述
输出一个整数，表示区间内因子数量为奇数的数的个数。

样例1
输入：
1 1

输出：
1

说明：区间内唯一可以取到的数字为 1，其因子数量只有自身，为奇数。

样例2
输入：
4 5

输出：
1

说明：区间内只有 4 的因子数量为奇数。
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  bool IsSquare(int &num) {
    for (int i = 1; i * i <= num; ++i) {
      if (i * i == num) return true;
    }
    return false;
  }
};

int main() {
    string input;
    getline(cin, input);
    stringstream ss(input);
    int n;
    int total = 0;
    Solution sol;
    while (ss >> n) {
      if (sol.IsSquare(n)) {
        total++;
      }
    }
    cout << total << endl;
}
