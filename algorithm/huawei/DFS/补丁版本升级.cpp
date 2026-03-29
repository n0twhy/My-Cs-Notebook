/*
题目：补丁版本升级

题目内容
某测试工具升级时总选择迭代次数最多的补丁版本。已知这些补丁版本的前序版本（即依赖该版本修改发布新补丁版本），前序版本的个数 <= 1，且不会存在互为前序版本的情况。

请给出最终可以升级的补丁版本。版本号只包含大写字母和数字。

输入描述
第一行为记录的版本迭代关系个数 N，范围是 [1, 100000]。

第二行到第 N+1 行：每行包含两个字符串，第一个字符串为当前版本，第二个字符串为前序版本，用空格隔开。字符串长度在 [1, 100]；没有前序版本时第二个字符串固定为 NA。

输出描述
输出所有迭代次数最多的补丁版本号字符串列表，多个版本号以字典序升序排列，用空格隔开。

样例1
输入：
6
CN0010 BF0001
BF0001 AZ0001
AZ0001 NA
BF0010 AZ0001
AW0001 NA
BF0011 AZ0001

输出：
CN0010

说明：
AZ0001 和 AW0001 没有前序版本，各迭代了 0 次；
BF0001、BF0010、BF0011 的前序版本为 AZ0001，各迭代了 1 次；
CN0010 的前序版本为 BF0001，BF0001 的前序版本为 AZ0001，迭代了 2 次。
根据要求选择迭代次数最多的补丁版本，因此输出 CN0010。

样例2
输入：
3
BF0001 AZ0001
AZ0001 NA
BF00011 AZ0001

输出：
BF0001 BF00011

说明：
AZ0001 没有前序版本，迭代了 0 次；
BF0001 和 BF00011 的前序版本为 AZ0001，各迭代了 1 次；
有多个版本号时以字典序排列，因此输出 BF0001 BF00011。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
	int dfs(string& curr_edition) {
		string prev_edition = mp[curr_edition];
		if (prev_edition == "NA") return 0;
		
		int size = 1;
		size += dfs(prev_edition);
		
		return size;
	}

	vector<string> PrevEdition(vector<pair<string, string>> &editions) {
		vector<pair<int, string>> iter_cnt;
		for (auto &[edition, prev_edition] : editions) {
			mp[edition] = prev_edition; 
		}
		for (auto &[edition, prev_edition] : editions) {
			int size = dfs(edition);
			iter_cnt.emplace_back(size, edition);
		}
		sort(iter_cnt.begin(), iter_cnt.end(), [](const pair<int, string> &a, const pair<int, string> &b){
			if (a.first != b.first) return a.first > b.first;
			return a.second < b.second;
		});

		vector<string> res;
		res.push_back(iter_cnt[0].second);
		int i = 1;
		while (i < iter_cnt.size() && iter_cnt[i].first == iter_cnt[i - 1].first) {
			res.push_back(iter_cnt[i].second);
			++i;
		}
		return res;
	}
private:
	map<string, string> mp;
};
int main() {
    int n;
		cin >> n;
		cin.ignore();
		vector<pair<string, string>> editions;
		
		for (int i = 0; i < n; ++i) {
			string input;
			getline(cin, input);
			stringstream ss(input);
			string curr, prev;
			ss >> curr >> prev;
			editions.emplace_back(curr, prev);
		}


		Solution sol;
		vector<string> res = sol.PrevEdition(editions);

		for (int i = 0; i < res.size(); ++i) {
			cout << res[i] << " ";
		}
}
