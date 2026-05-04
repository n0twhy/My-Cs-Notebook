/*
题目：地铁耗时最短的线路

题目内容
大湾区某城市地铁线路非常密集，乘客很难一眼看出选择哪条线路乘坐比较合适。为了解决这个问题，地铁公司希望你开发一个程序帮助乘客挑选合适的乘坐线路，使得乘坐时间最短。地铁公司可以提供的数据是各相邻站点之间的乘坐时间。

输入描述
第一行：N，站点总数（3 <= N <= 20）。

第二行：乘客的出发和到达站点（两个站点名，空格分隔）。

第三行起：相邻站点之间的乘坐时间，每对站点一行，格式为「站点1 站点2 耗时」。站点名称是单个小写字母，站点名一定包括出发和到达站点；输入保证只有一个唯一解。

结束行：
0000

输出描述
输出耗时最短的线路（站点名以空格分隔，按经过顺序输出）。

样例1
输入：
12
a e
a b 2
b c 2
c d 2
d e 2
f b 3
b g 3
g h 2
h i 3
j h 2
h e 3
e k 2
k l 4
0000

输出：
a b c d e

说明：线路 a->b->c->d->e 耗时最短。

样例2
输入：
12
f k
a b 2
b c 2
c d 2
d e 2
f b 3
b g 3
g h 2
h i 3
j h 2
h e 3
e k 2
k l 4
0000

输出：
f b c d e k

说明：线路 f->b->c->d->e->k 耗时最短。
*/

#include <bits/stdc++.h>
#include <climits>
#include <functional>
#include <iterator>
#include <vector>
using namespace std;

/*class Solution {
public:
	vector<char> MinimalPath(int station_count, char start, char end, vector<string> stations) {
		unordered_map<char, vector<pair<int, char>>> mp;
		vector<int> dijkstra(26, INT_MAX);
		unordered_set<char> mark; 
		vector<char> prev(26);
		priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;

		dijkstra[start - 'a'] = 0;

		for (auto& s : stations) {
			stringstream ss(s);
			char c;
			ss >> c;
			char next;
			ss >> next;
			int distance;
			ss >>distance;
			mp[c].emplace_back(distance, next);
			mp[next].emplace_back(distance, c);
		}

		for (auto &[distance, next_station] : mp[start]) {
			pq.push({distance, next_station});
			dijkstra[next_station - 'a'] = distance;
			prev[next_station - 'a'] = start;
		}
		mark.insert(start);


		for (int i = 0; i < station_count - 1 && !pq.empty(); ++i) {
			char curr_c = pq.top().second;
			pq.pop();
			for (auto &[distance, next_station] : mp[curr_c]) {
				if (mark.count(next_station)) continue;
				if (dijkstra[next_station - 'a'] > dijkstra[curr_c - 'a'] + distance) {
					dijkstra[next_station - 'a'] = 	dijkstra[curr_c - 'a'] + distance;
					prev[next_station - 'a'] = curr_c;
					pq.push({distance, next_station});
				}
			}
			mark.insert(curr_c);
		}

		vector<char> path;
		char curr = end;
		while (curr != start) {
			path.insert(path.begin(), curr);
			curr = prev[curr - 'a'];
		}
		path.insert(path.begin(), start);

		return path;
	}
};
*/

/*class Solution {
public:
	void dfs(char curr, char end, vector<char> &path, vector<bool> &visited, int cost) {
		if (curr == end) {
			if (cost < best_cost_) {
				best_cost_ = cost;
				best_path_ = path;
			}
			return;
		}

		for (auto &[distance, next_station] : mp[curr]) {
			if (visited[next_station - 'a']) continue;
			visited[next_station - 'a'] = true;
			path.push_back(next_station);
			dfs(next_station, end, path, visited, cost +distance);
			path.pop_back();
			visited[next_station - 'a'] = false;
		}
	}
	vector<char> MinimalPath(int station_count, char start, char end, vector<string> stations) {
		for (auto& s : stations) {
			stringstream ss(s);
			char c;
			ss >> c;
			char next;
			ss >> next;
			int distance;
			ss >>distance;
			mp[c].emplace_back(distance, next);
			mp[next].emplace_back(distance, c);
		}

		vector<bool> visited(26, false);
		visited[start - 'a'] = true;
		vector<char> path;
		path.push_back(start);
		dfs(start, end, path, visited, 0);

		return best_path_;
	}

private:
	unordered_map<char, vector<pair<int, char>>> mp;
	int best_cost_{INT_MAX};
	vector<char> best_path_;
};



	int main() {
		int n;
		cin >> n;
		char start, end;
		cin >> start >> end;
		cin.ignore();
		
		string input;
		vector<string> stations;
		while (getline(cin, input)) {
			if (input == "0000") break;
			stations.emplace_back(input);
		}

		Solution sol;
		vector<char> path = sol.MinimalPath(n, start, end, stations);

		for (int i = 0; i < path.size(); ++i) {
			cout <<path[i] << " ";
		}
	}*/

int main() {
	int n;
	cin >> n;
	char start, end;
	cin >> start >> end;
	cin.ignore();

	string input;
	unordered_map<char, vector<pair<char, int>>> graph;
	while (getline(cin, input)) {
		if (input == "0000") break;
		char u, v;
		int dist;
		stringstream ss(input);
		ss >> u >> v >> dist;
		
		graph[u].emplace_back(v, dist);
	}
	vector<char> curr_path;
	vector<char> best_path;
	int best_cost = 0x3f3f3f3f;

	vector<bool> visited(26, false);
	function<void(char, int)> dfs = [&](char curr, int curr_cost){
		if (curr == end) {
			if (curr_cost < best_cost) {
				best_cost = curr_cost;
				best_path = curr_path;
				return;
			}
		}

		for (auto [v, dist] : graph[curr]) {
			if (visited[v - 'a']) continue;
			visited[v - 'a'] = true;
			int cost = curr_cost + dist;
			curr_path.emplace_back(v);
			dfs(v, cost);
			curr_path.pop_back();
			visited[v - 'a'] = false;
		}
	};

	curr_path.emplace_back(start);
	dfs(start, 0);
	
	for (int i = 0; i < best_path.size(); ++i) {
		cout << best_path[i] << " ";
	}

}
