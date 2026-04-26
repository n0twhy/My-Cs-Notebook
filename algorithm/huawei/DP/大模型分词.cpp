/*
题目内容
您正在为一种罕见语言构建专用的大语言模型。由于训练样本缺失，
传统 BPE 等标准分词器效果不佳，导致模型推理生成句子质量不理想。

幸运的是，语言学家为该语言的已知词根与词缀（统称“词元”或 Token）
标注了“置信度”分数，代表该词元作为独立单位的合理性；
同时还给出词元间的转移分数，表示当前词元选择对下一个词元置信度的影响。

任务是设计一个“最优分词器”：
将输入句子（不含空格的英文小写字符串）切分为一系列词元，
使所有词元得分与转移加分之和最大。

输入描述
1）第一行输入待分词字符串 text（仅包含英文小写字母）。
2）第二行输入词典词条数 n。
3）接下来 n 行：每行包含一个单词及其分值，空格分隔。
4）第 n+3 行输入转移分数条数 m。
5）随后 m 行：每行包含“起始词 下一个词 转移分数加分 X”，空格分隔。

参数范围
0 < len(text) <= 100
-100 <= 词典中单词得分 <= 100
-100 <= 词元转移分数 P <= 100
0 < n <= 100
输入字符串均为英文小写字母

输出描述
输出最高分词得分。
若根据已知词汇表无法完成整句切分，则返回 0。
约定：若切分结果中出现不在词汇表中的词，则最终得分为 0。

样例1
输入
applepie
2
pen 3
apple 10
2
pen apple 5
pie apple 2

输出
0

说明
text 不能与词典词条匹配出完整切分结果，无法计算有效得分。

样例2
输入
goodeats
4
good 15
goo 12
deats 14
eats 10
1
good eats -5

输出
26

说明
切分为 ["good","eats"] 的总分 = 15 + 10 - 5 = 20；
切分为 ["goo","deats"] 的总分 = 12 + 14 = 26；
所以最大得分为 26。
*/

#include <bits/stdc++.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  int largemodelsparser(string &text, vector<pair<string, int>> &scores, vector<vector<string>> &trans_score) {
    unordered_map<string, int> mp;
    map<pair<string, string>, int> trans;

    for (auto v : trans_score) {
      trans.insert({{v[0], v[1]}, stoi(v[2])});
    }
    
    for (auto &[word,  score] : scores) {
      mp[word] = score;
    }

    vector<pair<int, string>> dp(text.size() + 1, {0, ""});
    dp[0].first = 1;

    for (int i = 1; i <= text.size(); ++i) {
      for (int j = 0; j < i; ++j) {
        auto &[score, s] = dp[j];
        if (score && mp.count(text.substr(j, i - j))) {
          int add = 0;
          auto it = trans.find({s, text.substr(j, i - j)});
          if (it != trans.end()) add += it->second;
          dp[i].first = max(score + mp[text.substr(j, i - j)] + add, dp[i].first);
          dp[i].second = text.substr(j, i - j);
        }
      }
    }

    if (dp[text.size()].first) return dp[text.size()].first - 1;
    return 0;
  }
};

int main() {
  string text;
  cin >> text;
  int n;
  cin >> n;
  vector<pair<string , int>> scores(n);
  for (int i = 0; i < n; ++i) {
    cin >> scores[i].first >> scores[i].second;
  }

  int m;
  cin >> m;

  vector<vector<string>> trans_score(m, vector<string>(3));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < 3; ++j) {
      cin >> trans_score[i][j];
    }
  }

  Solution sol;
  cout << sol.largemodelsparser(text, scores, trans_score) << endl;
}