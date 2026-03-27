/*
题目：打卡

题目描述
某高科技公司为员工提供智慧打卡系统，记录员工每日的进出门禁时间。为计算员工的实际工作时长，考虑以下规则：
1. 离岗时间不超过 15 分钟的，不扣除工作时长；
2. 午休时间为 12:00 至 14:00，此时间段内不算工作时长；
3. 晚饭时间为 18:00 至 19:30，此时间段内也不算工作时长。

输入描述
输入包括四行：
第一行是员工当天进门禁的次数 n；
第二行是员工当天所有的入门禁时间，以空格分隔（格式为 HH:MM）；
第三行是员工当天出门禁的次数 m；
第四行是员工当天所有的出门禁时间，以空格分隔（格式为 HH:MM）。

输出描述
输出为员工当天的工作时长（单位：分钟）。

样例输入
10
02:19 06:39 07:52 14:42 16:53 18:01 19:12 20:54 22:31 23:27
10
02:45 07:17 13:37 15:52 16:56 19:10 19:21 22:15 22:34 23:38

样例输出
480
*/

#include <bits/stdc++.h>
#include <ostream>
#include <string>
#include <utility>
using namespace std;

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 辅助函数：将 "HH:MM" 转换成绝对分钟数
    int TimeToMins(const string& time) {
        int h = stoi(time.substr(0, 2));
        int m = stoi(time.substr(3, 2));
        return h * 60 + m;
    }

    int WorkTime(vector<string> &ins, vector<string> &outs) {
        int n = ins.size();
        if (n == 0) return 0;

        // 1. 解析时间，处理可能跨越零点的情况 (比如 23:50 进，00:10 出)
        vector<pair<int, int>> intervals;
        for (int i = 0; i < n; ++i) {
            int in_time = TimeToMins(ins[i]);
            int out_time = TimeToMins(outs[i]);
            if (out_time < in_time) {
                out_time += 24 * 60; // 跨天补上 1440 分钟
            }
            intervals.push_back({in_time, out_time});
        }

        // 2. 合并 离岗 <= 15 分钟的区间
        vector<pair<int, int>> merged;
        int cur_in = intervals[0].first;
        int cur_out = intervals[0].second;

        for (int i = 1; i < n; ++i) {
            int next_in = intervals[i].first;
            int next_out = intervals[i].second;

            // 处理打卡记录可能整体跨越到第二天的问题
            if (next_in < cur_out) {
                next_in += 24 * 60;
                next_out += 24 * 60;
            }

            // 如果离岗时间 (next_in - cur_out) <= 15，合并区间
            if (next_in - cur_out <= 15) {
                cur_out = max(cur_out, next_out);
            } else {
                merged.push_back({cur_in, cur_out});
                cur_in = next_in;
                cur_out = next_out;
            }
        }
        merged.push_back({cur_in, cur_out}); // 别忘了推入最后一个合并好的区间

        // 3. 统计有效工作时长 (避开午饭 12:00-14:00 和晚饭 18:00-19:30)
        int total_time = 0;
        int lunch_start = 12 * 60, lunch_end = 14 * 60;
        int dinner_start = 18 * 60, dinner_end = 19 * 60 + 30;

        for (const auto& p : merged) {
            for (int t = p.first; t < p.second; ++t) {
                int mod_t = t % (24 * 60); // 映射回当天的标准 24 小时内

                // 判断当前分钟是否在休息时间内
                if (mod_t >= lunch_start && mod_t < lunch_end) continue;
                if (mod_t >= dinner_start && mod_t < dinner_end) continue;

                total_time++; // 有效工作分钟
            }
        }

        return total_time;
    }
};

int main() {
    int n;
    while (cin >> n) { // 支持多组测试用例
        vector<string> in(n);
        for (int i = 0; i < n; ++i) cin >> in[i];
        
        int m;
        cin >> m; // 通常 m == n，但需要读取掉这个值
        vector<string> out(m);
        for (int i = 0; i < m; ++i) cin >> out[i];
        
        Solution sol;
        cout << sol.WorkTime(in, out) << endl;
    }
    return 0;
}
