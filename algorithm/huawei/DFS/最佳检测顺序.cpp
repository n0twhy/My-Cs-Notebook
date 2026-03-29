/*
题目：最佳检测顺序

题目内容
小明有一家云存储服务提供商，他们家的核心产品是一个可扩展的分布式存储系统。客户使用他们的服务来存储和管理各种数据，包括文档、图片、视频等。由于客户对数据的可靠性和可用性要求非常高，他们需要提供高可用性的存储服务，以确保在任何情况下都能保持服务的可用性。

为了实现高可用性，他们使用了主备模式来管理他们的存储系统。当主节点发生故障时，系统会自动将业务切换到备用节点。为了保证存储系统的稳定性，他们需要及时检测服务状态，并在必要时触发主备切换。

在存储系统中，不同的服务之间存在依赖关系，每个服务最多只会依赖一个其他服务并且保证依赖不成环。例如，某些服务可能需要访问其他服务的数据才能正常工作。因此，当某个服务发生故障时，它所依赖的服务也会受到影响，可能导致更多的服务发生故障。

为了最大限度地减少服务故障对业务的影响，他们需要优先检测对业务影响大的服务，并按照节点编号升序编排检测顺序，现在请你帮忙解决一下这个问题。

注意：如果业务影响相同时，则按节点编号大小升序编排。

输入描述
第一行输入两个整数 n，代表业务节点总个数（1 <= n <= 100000）。

接下来一行 n 个整数，第 i 个整数 f_i 代表节点 i 所依赖的节点编号（i ∈ [0, n-1]）。

1. 若 f_i = -1，则代表没有节点依赖；
2. 数据保证 f_i != i。

输出描述
一行 n 个整数，以空格隔开，代表最终的排序结果。

样例1
输入：
5
-1 -1 1 2 3

输出：
1 2 3 0 4
*/

#include <bits/stdc++.h>
#include <iterator>
using namespace std;

class Solution {
public:
	int dfs(int u) {
		int size = 1;

		for (int i = 0; i < children_[u].size(); ++i) {
			size += dfs(children_[u][i]);
		}
		subtree_size_[u] = size - 1;
		return size;
	}
	vector<int> CloudServers(vector<int> &cloud_servers) {
		subtree_size_.resize(cloud_servers.size());
		children_.resize(cloud_servers.size());
		for (int i = 0; i < cloud_servers.size(); ++i) {
			if (cloud_servers[i] == -1) continue;
			children_[cloud_servers[i]].emplace_back(i);
		}
		vector<pair<int, int>> res(cloud_servers.size());
		
		for (int i = 0; i < cloud_servers.size(); ++i) {
			if (cloud_servers[i] == -1) {
					dfs(i);
			}
	}
		
		for (int i = 0; i < cloud_servers.size(); ++i) {
			res[i] = {i, subtree_size_[i]};
		}

		sort(res.begin(), res.end(), [](const pair<int, int>& a, const pair<int, int>& b){
			if (a.second != b.second) return a.second > b.second;
			return a.first < b.first;
		});

		vector<int> ans;
		for (auto &[node, dist] : res) {
			ans.emplace_back(node);
		}
		return ans;

		
	}
private:
	vector<vector<int>> children_;
	vector<int> subtree_size_;
};

int main() {
	int n;
	cin >> n;
	vector<int> input(n);
	for (int i = 0; i < n; ++i) {
		cin >> input[i];
	}

	Solution sol;
	vector<int> res = sol.CloudServers(input);
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << " ";
	}
}
