/*
题目：云服务GPU分配

题目描述
某云计算服务商为客户提供 M 数量 GPU 核数的 GPU 分时租用服务，租用计费规则为：
允许客户在每个时间单位按需租用不同的 GPU 核数，每个时间单位每个 GPU 核数的费用为 R。

现有 N 个客户，每个客户有多个不重叠时间段租用一定数量的 GPU 核数的租用需求。
对于有租用需求的客户，服务商可选择签约或不签约，若选择签约则需要满足租用需求中的
所有时间段所需的 GPU 核数。

为了实现租金最大化收益，服务商需在确保任意时间单位内分配的 GPU 核数总数不超过 M
的基础上，选择与哪些客户签约租用协议。

请输出租金最大化收益下的租金最大值。

输入描述
第一行为 M、N、R 的数值，依次用空格隔开，输入格式为：
M N R

从第二行开始，每行为一个客户的租用需求，共 N 行。
每行第一个数字为该客户端的时间段个数 timeSegmentNum，后续为 timeSegmentNum 个时间段
及所需的 GPU 核数。时间段个数与时间段之间、多个时间段之间均用空格分割。
同一个客户多个时间段不会重叠，且已按起始时间增序排序给出。

每个时间段及所需 GPU 核数格式为：
start:end:needCores
- start：起始时间编号
- end：结束时间编号
- needCores：该时间段所需 GPU 核数

变量取值范围
1 <= M <= 100000
1 <= N <= 10
1 <= R <= 10
0 <= start <= end <= 10^9
1 <= needCores <= 10000
1 <= timeSegmentNum <= 100

客户租用需求样例：
2 0:0:1 3:6:10
含义是共有 2 个时间段，
0:0:1 表示在第 0 个时间单位需要 1 个 GPU 核，
3:6:10 表示在从 3 到 6 的时间单位（包含 3 和 6）每个时间单位均需 10 个 GPU 核。

输出描述
输出总租金最大值。

如果任意一个客户的需求都无法满足，则输出 0。

样例1
输入：
10 3 2
2 0:8:5 9:23:10
2 0:8:5 9:18:10
1 0:8:5

输出：
480

说明：
共 3 个客户。
由于第一个客户和第二个客户在 9:18 时间范围段内总核数为 20，超过了 10，所以无法同时接受。
最大日租金方案为：接纳第一个客户和第三个客户的需求。
第一个客户共需要的 GPU 核数为 9*5 + 15*10 = 195
第三个客户共需要的 GPU 核数为 9*5 = 45
最大日租金值为 (195 + 45) * 2 = 480

样例2
输入：
10 2 1
1 0:3:6
1 3:10:6

输出：
48

说明：
最大 GPU 核数为 10，共 2 个客户。
第一个客户和第二个客户在 3 时间点，总核数为 12，超过了 10，所以无法同时接受。
第一个客户共需要的 GPU 核数为 4*6 = 24
第二个客户共需要的 GPU 格数为 8*6 = 48
为满足最大租金，采纳第二个客户，最大日租金值为 48*1 = 48

样例3
输入：
10 1 1
1 0:5:20

输出：
0

说明：
最大 GPU 核数为 10，共 1 个客户。
在 0~5 时间段需要 20 个 GPU 核数，无法满足。
输出 0。
*/

#include <bits/stdc++.h>
using namespace std;

struct Segment {
    long long s;
    long long e;
    long long need;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long M, R;
    int N;
    cin >> M >> N >> R;

    vector<vector<Segment>> clients(N);
    for (int i = 0; i < N; ++i) {
        int k;
        cin >> k;
        clients[i].reserve(k);
        for (int j = 0; j < k; ++j) {
            string token;
            cin >> token;  // start:end:need

            size_t p1 = token.find(':');
            size_t p2 = token.find(':', p1 + 1);
            long long s = stoll(token.substr(0, p1));
            long long e = stoll(token.substr(p1 + 1, p2 - p1 - 1));
            long long need = stoll(token.substr(p2 + 1));
            clients[i].push_back({s, e, need});
        }
    }

    long long bestCoreTime = 0;
    int totalMask = 1 << N;

    for (int mask = 0; mask < totalMask; ++mask) {
        map<long long, long long> diff;

        for (int i = 0; i < N; ++i) {
            if ((mask & (1 << i)) == 0) {
                continue;
            }
            for (const auto &seg : clients[i]) {
                diff[seg.s] += seg.need;
                diff[seg.e + 1] -= seg.need;
            }
        }

        if (diff.empty()) {
            bestCoreTime = max(bestCoreTime, 0LL);
            continue;
        }

        long long curNeed = 0;
        long long lastTime = 0;
        bool hasLast = false;
        long long coreTime = 0;
        bool ok = true;

        for (const auto &kv : diff) {
            long long t = kv.first;
            if (hasLast) {
                long long len = t - lastTime;
                if (curNeed > M) {
                    ok = false;
                    break;
                }
                coreTime += curNeed * len;
            }
            curNeed += kv.second;
            lastTime = t;
            hasLast = true;
        }

        if (ok && curNeed == 0) {
            bestCoreTime = max(bestCoreTime, coreTime);
        }
    }

    cout << bestCoreTime * R << '\n';
    return 0;
}
