/*
 * 多段数据下发（配置/删除区间合并）
 * =============================================================================
 *
 * 【题目描述】
 * 在数通设备进行配置下发时，可能会遇到需要下发一个或多个数据段的场景。
 * 为简化用户操作，数据下发同时支持“散列”及“段下发”模式，且数据段的顺序不做要求。
 *
 * 下发后的数据会被整理、合并并保存到数据库中，合并结果满足：
 *  1) 数据段无法继续合并
 *  2) 数据段从小到大排列
 *  3) 区间长度为 1 时，使用单个数字形式保存（例如：15 表示 [15,15]）
 *
 * 【配置操作】
 * 数据库中已有数据时，新配置不会覆盖已有数据，而是把“新增区间集合”与“数据库已有区间集合”
 * 取并集后整理合并更新。
 *
 * 【删除操作】
 * 当用户下发删除配置时，从已有范围中删除与下发范围重合的部分。
 * 即：数据库区间集合减去“待删除区间集合”覆盖到的部分，再整理合并更新。
 *
 * =============================================================================
 *
 * 【输入描述】
 * 第一行：一个整数 n（下发/删除操作的数量），n <= 100
 *
 * 接下来 n 行：每行包含一个字符串，表明一次操作。
 *  - 配置操作： "algorithm 数据"
 *  - 删除操作： "undo algorithm 数据"
 *
 * 【数据格式描述】
 * 数据由若干个“原子数据”通过逗号 ',' 连接组成：
 *   a-b,c,d-e
 * 原子数据有两种形式：
 *  1) 单个正整数 a（表示区间 [a,a]）
 *  2) 正整数 a-b（a <= b，表示区间 [a,b]）
 *
 * 每个数的范围为：1 <= a,b <= 65536
 *
 * 【注】
 *  1) 输入保证合法；原子数据段的顺序不保证已排序。
 *  2) 每次操作的原子数据段数量不超过 100。
 *  3) 数据库数据为空时，删除操作视为无效操作（结果仍为空）。
 *
 * =============================================================================
 *
 * 【区间合并规则】
 * 给定任意两个区间 [l1,r1] 与 [l2,r2]（假设 l1 <= l2）：
 *  - 若它们相交或相邻（即 l2 <= r1+1），则可以合并为 [l1, max(r1,r2)]
 *  - 合并直到无法再合并为止
 *
 * 输出时：
 *  - 区间长度为 1：输出 "a"
 *  - 区间长度 >= 2：输出 "a-b"
 *  - 多个区间用逗号 ',' 分隔，按区间左端点从小到大输出
 *
 * =============================================================================
 *
 * 【输出描述】
 * 输出 1 个字符串，表示最终数据库整理合并后的结果（格式与输入的数据格式相同）。
 * 若最终数据库为空，输出 0
 *
 * =============================================================================
 *
 * 【样例 1】
 * 输入：
 * 2
 * algorithm 1-10,15-20
 * algorithm 5-11
 *
 * 输出：
 * 1-11,15-20
 *
 * 【样例 2】
 * 输入：
 * 3
 * undo algorithm 1-100
 * algorithm 15-20,1-10
 * undo algorithm 6,7,8,9-10
 *
 * 输出：
 * 1-5,15-20
 *
 * =============================================================================
 */

#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  string databaseOutput(vector<string>& input) {
    vector<pair<int, int>> db;

    for (auto& s : input) {
      vector<pair<int, int>> operation;
      bool is_undo = (s.substr(0, 4) == "undo");
      
      string data = s.substr(s.rfind(' ') + 1);
      stringstream ss(data);
      string token;

      while (getline(ss, token, ',')) {
        bool is_range = (token.find('-') != string::npos);
        if (is_range) {
          int first = stoi(token.substr(0, token.find('-')));
          int second = stoi(token.substr(token.find('-') + 1));
          operation.emplace_back(first, second);
        } else {
          int num = stoi(token);
          operation.emplace_back(num, num);
        }
      }

      if (!is_undo) {
        for (auto& p : operation) {
          db.push_back(p);
          db = merge(db);
        }
      } else {
        for (auto& p : operation) {
          db = substract(db, p);
        }
      }
    }

    if (db.empty()) return "0";
    stringstream res;
    for (int i = 0; i < db.size(); ++i) {
      if (i > 0) res << ",";
      if (db[i].first == db[i].second) {
        res << db[i].first;
      } else {
        res << db[i].first << "-" << db[i].second;
      }
    }
    return res.str();
    
  }

  vector<pair<int, int>> merge(vector<pair<int, int>>& v) {
    sort(v.begin(), v.end());
    vector<pair<int, int>> tmp;
    for (auto& p : v) {
      if (tmp.empty() || p.first > tmp.back().second + 1) {
        tmp.emplace_back(p);
      } else {
        tmp.back().second = max(tmp.back().second, p.second);
      }
    }
    return tmp;
  }

  vector<pair<int, int>>  substract(vector<pair<int, int>>& db, pair<int, int>& p) {
    vector<pair<int, int>> tmp;
    for (auto& [l, r] : db) {
      if (l > p.second || r < p.first) {
        tmp.emplace_back(l, r);
      } else {
        if (l < p.first) tmp.emplace_back(l, p.first - 1);
        if (r > p.second) tmp.emplace_back(p.second + 1, r);
      }
    }
    return tmp;
  }
};

int main() {
  int n;
  cin >> n;
  cin.ignore();
  vector<string> input(n);
  for (int i = 0; i < n; ++i) {
    getline(cin, input[i]);
  }
  Solution sol;
  cout << sol.databaseOutput(input) << endl;
}


