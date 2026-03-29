/*
题目：栈大小峰值计算

题目内容
给定一系列函数调用关系，以及沿每条调用边所需开辟的栈内存大小。要求输出：

1. 系统运行时所需的最大栈大小：某条调用链上，各次调用所开辟栈内存的总和的最大值。

2. 在满足该栈总和最大的前提下，这条调用链上函数个数的最大值（若多条链栈总和同为最大，取其中函数个数最多的那条）。

输入描述
第一行：整数 M，表示调用关系条数。

接下来 M 行：每行三个整数 caller callee size，表示函数 caller 调用 callee 时，该次调用关系需开辟 size 字节栈空间。

示例（对应 2 条边）：
2
0 1 128
1 2 128
表示：0 调用 1 开辟 128；1 调用 2 再开辟 128。

约束
- 函数 id：0 <= id <= 999
- 栈大小：8 <= size <= 10240
- 调用关系条数 M <= 1000

输出描述
1. 若存在环（递归调用关系），输出 -1。

2. 若无环：输出两个整数，空格分隔——第一个为最大栈内存总和，第二个为在达到该最大总和时，调用链上的最大函数个数（路径上的节点数）。

样例1
输入：
3
0 1 128
1 2 128
2 0 128

输出：
-1

说明：存在环 0->1->2->0，输出 -1。

样例2
输入：
3
0 1 128
1 2 128
1 3 32

输出：
256 3

说明：
路径 0->1->2：栈总和 128+128=256，链上 3 个函数。
路径 0->1->3：栈总和 128+32=160，链上 3 个函数。
最大栈为 256，此时链长为 3，输出 256 3。
*/

#include <bits/stdc++.h>
using namespace std;

/*class Solution {
public:
  pair<int, int> FuncCalling(vector<vector<int>> calls) {
		
		// 拓扑排序 + DP 求最长路径（带路径节点数）
		// dp[u].first = 到达 u 的最大权重和
		// dp[u].second = 达到最大权重和时的节点个数

		unordered_map<int, vector<pair<int,int>>> graph;  // graph[u] = {{v, w}, ...}
		unordered_map<int, int> indegree;
		unordered_map<int, pair<int,int>> dp;  // {最大距离, 节点个数}

		// 1. 建图，统计入度
		for (auto& call : calls) {
				int u = call[0], v = call[1], w = call[2];
				graph[u].push_back({v, w});
				indegree[v]++;
				if (!indegree.count(u)) indegree[u] = 0;
		}

		// 2. 初始化：入度为0的入队，节点个数初始为1（自身）
		queue<int> q;
		for (auto& [node, deg] : indegree) {
				dp[node] = {0, 1};
				if (deg == 0) q.push(node);
		}

		// 3. 拓扑排序 + DP
		int processed = 0;
		while (!q.empty()) {
				int u = q.front(); q.pop();
				processed++;
				for (auto [v, w] : graph[u]) {
						int new_dist = dp[u].first + w;
						int new_cnt = dp[u].second + 1;
						if (new_dist > dp[v].first) {
								dp[v] = {new_dist, new_cnt};
						} else if (new_dist == dp[v].first) {
								dp[v].second = max(dp[v].second, new_cnt);
						}
						if (--indegree[v] == 0) q.push(v);
				}
		}

		// 4. 环检测
		if (processed < indegree.size()) return {-1, -1};

		// 5. 找答案
		int max_dist = 0, max_cnt = 0;
		for (auto& [node, val] : dp) {
				if (val.first > max_dist) {
						max_dist = val.first;
						max_cnt = val.second;
				} else if (val.first == max_dist) {
						max_cnt = max(max_cnt, val.second);
				}
		}
		return {max_dist, max_cnt};
	}
private:
	unordered_map<int, vector<pair<int, int>>> mp;
	unordered_map<int, int> indegree;
};*/

class Solution {
public:
	void dfs(int u) {
		visited[u] = 1;
		for (auto &[v, w] : graph[u]) {
			if (visited[v] == 1) {
				has_circle_ = true;
				return;
			}
			
			if (visited[v] == 0) dfs(v);
			
			if (has_circle_) {
				return;
			}
			int new_dist = dp[v].first + w;
			int new_cnt = dp[v].second + 1;
			if (new_dist > dp[u].first) {
				dp[u] = {new_dist, new_cnt};
			} else if (new_dist == dp[u].first) {
				dp[u].second = max(dp[u].second, new_cnt);
			}
		}
		visited[u] = 2;
	}
	
	pair<int, int> FuncCalling(vector<vector<int>> calls) {
		for (auto& call : calls) {
			int u = call[0], v = call[1], w = call[2];
			graph[u].push_back({v, w});
			dp[u] = {0, 1};
			dp[v] = {0, 1};
		}

		for (auto &it : dp) {
			dfs(it.first);
		}
		if (has_circle_) return {-1, -1};
		int best_size = 0;
		int best_cnt = 0;
		for (auto &[key, value] : dp) {
			if (value.first > best_size) {
				best_size = value.first;
				best_cnt = value.second;
			} else if (value.first == best_size) { 
				best_cnt = max(best_cnt, value.second);
			}
		}

		return {best_size, best_cnt};
		
	}

private:
	unordered_map<int, vector<pair<int,int>>> graph;  // graph[u] = {{v, w}, ...}
	unordered_map<int, pair<int,int>> dp;
	unordered_map<int, int> visited;
	bool has_circle_ {false};
};

int main() {
    int n;
		cin >> n;
		vector<vector<int>> input(n, vector<int>(3));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < 3; ++j) {
				cin >> input[i][j];
			}
		}

		Solution sol;
		auto res = sol.FuncCalling(input);
		if (res.first == -1) cout << "-1" << endl;
		else cout << res.first << " " << res.second;
}
