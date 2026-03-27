/*
题目：海量日志抑制

题目描述
小明的朋友是一位从事运维工作的专业人士。有一天，他突然有急事需要请假，但是他非常担心公司的系统运行日志出现海量日志的问题。
这种问题是指系统打印了大量相同或相似内容的日志，导致有效信息难以被捕捉，甚至会影响系统的运行效率。
为了避免这种情况发生，小明的朋友请求小明帮助管理系统的运行日志，并确保只记录有用的信息，避免无效日志的产生。

小明针对海量日志的问题，提出了一种智能算法机制，避免在系统运行时产生大量日志。在这个问题中，
"海量日志" 定义如下：
1. 在 10 毫秒内（小于 10 毫秒），如果打印了 2 条相同的日志，只保留第一条；
2. 在 100 毫秒内（小于 100 毫秒），如果打印了 10 条相似的日志，只保留前 9 条。

按时间读取日志，若被读取的日志被判定为抑制日志，则其将不会记录到日志文件中，即删去这一项。

字符串 s、t 相似的定义：
去除掉两者中所有数字后（相对顺序不发生改变）逐字符相等，则 s、t 相似。

为简化题意，给定输入满足：
- 后一条日志的时间戳不小于前一条；
- 时间戳取值范围 [1, 10000]；
- 日志内容长度不超过 1000；
- 所有数字均为正整数；
- 本用例中的日志条数最多不超过 1000 条。

输入描述
输入日志条数 n，接下来 n 行每行一个日志，格式为：
timestamp:content

输出描述
按时间戳顺序输出被抑制后的日志（即最终保留的日志）。

样例1
输入：
5
100:1cbbb
100:2c3a2
102:2c3a2
102:2232c
103:2232c

输出：
102:2c3a2
103:2232c
*/

#include <bits/stdc++.h>
#include <iterator>
using namespace std;

class Solution {
public:
  string EliminateNum(string& s) {
    string res;
    
    for (auto &c : s) {
      if (isdigit(c)) continue;
      res.push_back(c);
    }
    
    return res;
  }


  pair<int, string> Sparse(string& log) {
    int index = log.find(':');
    int timestamp = stoi(log.substr(0, index));
    string name = log.substr(index + 1);
    return {timestamp, name};   
  }

  vector<string> LogRepresser(vector<string>& logs) {
    unordered_map<string, int> same_map;
    unordered_map<string, queue<int>> sim_map;
    vector<string> res;

    for (int i = 0; i < logs.size(); ++i) {
        // 优化：只调用一次解析函数
        pair<int, string> parsed = Sparse(logs[i]);
        int curr_ts = parsed.first;
        string name = parsed.second;
        string sim = EliminateNum(name);
        
        // 条件 1：检查 10ms 完全相同（注意是 curr - past < 10）
        if (same_map.count(name) && curr_ts - same_map[name] < 10) {
            res.push_back(logs[i]);
            continue; // 触发抑制，直接跳过，不用往下走了
        }

        // 条件 2：处理 100ms 相似日志
        // 先清理过期数据（注意是 >= 100 就过期）
        while (!sim_map[sim].empty() && (curr_ts - sim_map[sim].front() >= 100)) {
            sim_map[sim].pop();
        }

        // 再检查清理后的队列长度
        if (sim_map[sim].size() >= 9) {
            res.push_back(logs[i]);
            continue; // 触发抑制，直接跳过
        }

        // 只有没被 continue 拦截下来的合法日志，才能更新历史记录
        same_map[name] = curr_ts;
        sim_map[sim].push(curr_ts);
    }

    return res;
}
};

int main() {
  int n;
  cin >> n;
  vector<string> logs(n);

  for (int i = 0; i < n; ++i) {
    cin >> logs[i];
  }

  Solution sol;
  vector<string> res = sol.LogRepresser(logs);

  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << endl;
  }
}