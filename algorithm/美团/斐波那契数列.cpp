/*
题目：斐波那契数列（超级斐波那契）

题目内容
定义超级斐波那契数列如下：给定整数 k，该序列的前 k 项均为 1；
对于 n > k，第 n 项为前 k 项之和，即
S_n = S_{n-1} + S_{n-2} + ... + S_{n-k}

现给定整数 k 和查询次数 q，每次查询一个正整数 x，请输出该序列的第 x 项对 (10^9 + 7) 取模后的值。

输入描述
第一行输入两个整数 k, q（1 <= k <= 10^6；1 <= q <= 2 * 10^5）。

此后 q 行，每行输入一个正整数 x（1 <= x <= 10^6）。

输出描述
输出 q 行，每行输出一个整数，表示对应查询的答案对 (10^9 + 7) 取模后的值。

样例1
输入：
2 5
1
2
3
4
5

输出：
1
1
2
3
5

说明：
x=1 时，S_1 = 1；
x=2 时，S_2 = 1；
x=3 时，S_3 = S_2 + S_1 = 1 + 1 = 2；
x=4 时，S_4 = S_3 + S_2 = 2 + 1 = 3；
x=5 时，S_5 = S_4 + S_3 = 3 + 2 = 5。
*/

#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
  vector<int> helper(int k) {
    vector<int> array;
    int sum = k;
    for (int i = 0; i <= k; ++i) {
      array.push_back(1);
    }
    int l = 0;
    int r = k;

    for (int i = 0; i < 1000000; ++i) {
      sum -= array[l++];
      sum += array[r++];
      array.push_back(sum);
    }

    return array;
  }
};

int main() {
    int k, n;
    cin >> k >> n;
    Solution sol;
    vector<int> array = sol.helper(k);
    for (int i = 0; i < n; ++i) {
      int tmp;
      cin >> tmp;
      cout << array[tmp] << endl;
    }
}
