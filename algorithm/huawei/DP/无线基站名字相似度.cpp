/*
题目：无线基站名字相似度

题目内容
在无线通信中，每一个基站都会有一个名字，一般同一个区域的基站名字会比较相近，
可以通过判断两个基站名字相似程度来识别它是否在同一区域。
通过基站间的名字字符串之间转换，来判断两个基站名字的相似度。
字符之间的转换只有 3 种操作（增、删、改）：

1、增：插入一个字符；
2、删：删除一个字符；
3、改：替换一个字符；

并且以上 3 种操作分别对应不同的打分项，得分越低，说明相似度越高。

1) 增：3 分；
2) 删：3 分；
3) 替换：
   - 字符在以下两分组内同一组的得 1 分；
   - 分别在两个组的得 2 分；
   - 其他得 3 分。

组1：{'w','i','r','e','l','@','c','o','m'}
组2：{'h','f','v','#','g','b','t','s'}

给定两个无线基站名字，请识别出相似度（即字符转换操作的最低得分）。

输入描述
输入两个名字字符串。
注：字符串长度范围 [1,2000]。

输出描述
输出两者之间的相似度。

样例1
输入
chu
xu

输出
6

说明
基站名字1为 "chu"，基站名字2为 "xu"，进行这两个基站名字间的字符转换步骤：

第1步 c 替换为 x：chu -> xhu，
c 在组1内，x 不在，所以得分 3。

第2步 h 删除：xhu -> xu，得分 3。

总得分为 6，所以相似度为 6，输出 6。

样例2
输入
jinhailu
jinzhanglu

输出
8

说明
基站名字1为 "jinhailu"，基站名字2为 "jinzhanglu"。

路径1：
第1步 h 替换为 z：jinhailu -> jinzailu，
h 在组2，z 不在，所以得分 3。

第2步 i 替换为 n：jinzailu -> jinzanlu，
i 在组1，n 不在，所以得分 3。

第3步 插入 g：jinzanlu -> jinzanglu，得分 3。

总得分为 9。

路径2：
第1步 h 替换为 z：jinhailu -> jinzailu，
h 在组2，z 不在，所以得分 3。

第2步 插入 n：jinzailu -> jinzanilu，得分 3。

第3步 i 替换为 g：jinzanilu -> jinzanglu，
i 在组1，g 在组2，所以得分 2。

总得分为 8。

所以路径2得分更低，输出 8。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  int SimilarityScore(string &a, string &b) {
    string tmp_1 = "wirel@com";
    string tmp_2 = "hfv#gbts";
    for (char c : tmp_1) {
      group_[c] = 1;
    }
    for (char c : tmp_2) {
      group_[c] = 2;
    }

    vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1));
    dp[0][0] = 0;
    for (int i = 1; i <= b.size(); ++i) {
      dp[0][i] = dp[0][i - 1] + 3; 
    }

    for (int i = 1; i <= a.size(); ++i) {
      dp[i][0] = dp[i - 1][0] + 3;
    }

    for (int i = 0; i < a.size(); ++i) {
      for (int j = 0; j < b.size(); ++j) {
        if (a[i] == b[j]) {
          dp[i + 1][j + 1] = dp[i][j];
          
        } else {
          int replace_score = 3;
          int a_g = group_[a[i]];
          int b_g = group_[b[j]];
          if (a_g != 0 && b_g != 0) {
            if (a_g == b_g) replace_score = 1;
            else replace_score = 2;
          }
          dp[i + 1][j + 1] = min({dp[i][j + 1] + 3, dp[i + 1][j] + 3, dp[i][j] + replace_score});
        }
        cout << i + 1 << "~" << j + 1 << ": " << dp[i + 1][j + 1] << endl;
      }
    }
    return dp[a.size()][b.size()];
  }
private:
  unordered_map<char, int> group_;
};

int main() {
  string a, b;
  cin >> a >> b;
  Solution sol;
  cout << sol.SimilarityScore(a, b) << endl;
}

