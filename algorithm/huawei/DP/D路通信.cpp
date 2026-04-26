/*
题目内容
塔子与兔子两个兄弟刚学完计算机网络原理课程，他们对通信这一概念很感兴趣。
现在老师给了他们一个 D 路通信场景。通信链路有如下性质：

1. 高斯噪声性：
   如果发出一段字符串作为消息，消息的开始前和结束后可能会出现随机高斯噪声。
2. 内容完整性：
   该过程不会丢失任何字符，字符顺序也不会发生变化。
3. 字符统一性：
   所有的消息内容和噪声都是小写字符。

基于链路特点，他们想到了一种消除高斯噪声的算法：
1. 同时采用两条含有随机噪声的链路发出一段消息。
2. 在接收侧，在接收到的两条消息中寻找最长的那段连续公共子串，作为有效信息。

现在需要求有效消息的长度。注意：
- 有效消息不一定唯一；
- 也有可能为空；
- 只需要返回消息长度。

输入描述
输入共两行，分别代表两条链路收到的信息（两个字符串），仅包含小写字母。

约束范围：
0 < len <= 1000

输出描述
输出一行，一个数字（以回车结束），表示有效信息的长度。

样例一
输入
vsavvzxaaxvzvz
zzczcaaa

输出
2

样例解释
两条信息中，最长的公共字符串是 "aa"，长度为 2。

样例二
输入
tttazitazittz
tazittttt

输出
6

样例解释
两条信息中，最长的公共字符串是 "tazitt"，长度为 6。
*/
#include <bits/stdc++.h>
#include <climits>
#include <vector>
using namespace std;

class Solution {
public:
  int maxsubstring(string a,string b) {
    int m = a.size();
    int n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    int max_length = INT_MIN;
    dp[0][0] = 0;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (a[i] == b[j]) {
          dp[i + 1][j + 1] = dp[i][j] + 1;
        }
        max_length = max(max_length, dp[i + 1][j + 1]);
      }
    }
    return max_length;
  }
};

int main() {
  string a, b;
  cin >> a >> b;
  Solution sol;
  cout << sol.maxsubstring(a, b) << endl;
}