/*
题目内容
fs 拥有多张银行卡，每张卡的密码均由 6 个数字组成（可含前导 0）。密码太多不易记忆，他打算进行一次统一改动：选择同一个位置 pos ∈ {1,2,3,4,5,6}，并选择一个数字 d ∈ {0,1,...,9}，把所有卡在该位置的数字都改成 d。

改动完成后，得到一批新的 6 位数字密码。fs 希望不同密码的种类数尽可能少。请你计算：完成一次上述改动后，不同密码的最少种类数。

输入描述
每个测试文件包含多组测试数据。第一行输入一个整数 T(1 <= T <= 10^5) 表示测试组数。接下来每组数据描述如下：
第一行输入一个整数 n(1 <= n <= 10^5) 表示银行卡数量。
第二行输入 n 个长度为 6 的数字串，依次表示每张卡的原始密码。

保证所有测试数据的 n 之和不超过 2 * 10^5。

输出描述
对于每组测试数据，输出一行一个整数，表示进行一次统一改动后，不同密码的最少种类数。

样例1
输入
3
5
000000 100000 200000 300000 000000
3
123456 123556 123656
3
000000 111111 222222
输出
1
1
3
*/

#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
  int check(vector<string> &codes) {
    sort(codes.begin(), codes.end());
    codes.erase(unique(codes.begin(), codes.end()), codes.end());
    return codes.size();
  }

  int minimal() {
    int n;
    cin >> n;
    vector<string> codes(n);
    for (int i = 0; i < n; ++i) {
      cin >> codes[i];
    }

    int cnt = 0x3f3f3f3f;

    for (int i = 0; i < 6; ++i) {
      vector<string> backup = codes;
      for (auto &code : backup) {
        code[i] = '*';
      }
      cnt = min(cnt, check(backup));
    }

    return cnt;
    
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  Solution sol;
  for (int i = 0; i < n; ++i) {
    cout << sol.minimal() << "\n";
  }
}
