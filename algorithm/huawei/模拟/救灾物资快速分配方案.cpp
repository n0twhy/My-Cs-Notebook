/*
题目：救灾物资快速分配方案
某地发生地震，道路被毁，只有一条路可走。多个市派出车队运送物资，每个市提供的物资车数不一样。
所有市车队到达灾区后需要排队依次进入；每个市车队组成数组 cars[]。
同时灾区临时营地中有多人排队领取物资，领取队伍表示为数组 requires[]。
每个人的需求物资车数为 requires[j]。
为尽快缩减营地中排队领物资队伍的长度，制定发放规则如下：
假设当前进入营地的市车队运来的物资车数为 K（即营地当前累计拥有的物资车数）。
将营地中尚未被满足的领取人组成一段“领取队列”（按原相对顺序）。
规则 1：
在领取队伍中，找到“需求和 <= K”的最长连续子序列。
该子序列的所有人都将收到物资。
将营地中当前所有物资（共 K 车）分配给这个子序列中的所有人。
（仅要求子序列的需求和 <= K；子序列内人的需求之和不超过 K，则可以完成本次分配。）
一次分配完成后，营地中剩余未满足的人会保留在原队列中，且本次营地累计物资耗尽（物资不再带到下一次分配）。
规则 2：
如果规则 1 找不到任何满足“需求和 <= K”的领取人子队伍（即不存在满足条件的连续子序列，
等价于：所有单个领取人的需求都大于 K），
则把下一个市的车队加入营地：将该市物资车数与当前营地物资累计相加，更新 K，
并重新进行规则 1 的“最长连续子序列”计算。
流程结束条件：
当市车队全部进入营地后，若仍无法按照规则 1 完成分配，则流程结束。
此时所有未被分配到物资的领取人无法领取物资。
要求：
按照以上规则发放，总计进行了多少次分配，以及最终有多少人不能领到物资。
----------------------
输入描述
输入为两行：
第 1 行：各市物资车数的数组 cars，空格分隔。
cars[i] 表示编号 i（i <= 200）的市运来的物资车数，约束为：
1 <= cars[i] <= 5000。
第 2 行：需求数组 requires，空格分隔。
requires[j] 表示编号 j（j <= 1e5）的人的需求车数，约束为：
1 <= requires[j] <= 100。
若输入不满足要求，输出 “-1 -1”。
----------------------
输出描述
输出两项，用空格分隔：
1) 总计分配次数
2) 最终不能分到物资的人数
----------------------
示例 1
输入
8 7 3 6 6 2 1
7 1 2 4 6 1 2 3 1 4 5
输出
4 1
----------------------
示例 2
输入
8
4 2 2 8 2 2 2 1
输出
1 4
----------------------
示例 3
输入
2 2 2 2
8 9 8 8
输出
1 3
*/

#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <sstream>
using namespace std;

class Solution {
public:
	int allocate(vector<int>& requires, int k) {
		int l = 0;
		int sum = 0;
		int max_len = 0;
		int bestL = 0, bestR = 0;
		int size = requires.size();

		for (int r = 0; r < requires.size(); ++r) {
			sum += requires[r];
			
			while (l <= r && sum > k) {
				sum -= requires[l];
				l++;
			}

			int curr_len = r - l + 1;
			if (curr_len > max_len) {
				bestL = l;
				bestR = r;
				max_len = curr_len;
			}	
		}

		if (max_len == 0) return 0;

		for (int i = bestL, j = bestR + 1; j < requires.size(); ++i, ++j) {
			requires[i] = requires[j];
		}
		requires.resize(size - max_len);

		return max_len;
	}

	pair<int, int> allocation(vector<int> &cars, vector<int> requires) {
		queue<int> q;
		int allocate_times = 0;
		int rest_people = requires.size();

		for (int i = 0; i < cars.size(); ++i) {
			q.push(cars[i]);
		}
		int k = 0;
		while (!q.empty()) {
			k = q.front();
			q.pop();
			int len = allocate(requires, k);
			while (len == 0 && !q.empty()) {
				k += q.front();
				q.pop();
				len = allocate(requires, k);
			}
			if (len != 0) {
				allocate_times++;
				rest_people -= len;
			}
		}

		return {allocate_times, rest_people};
	}
};

int main() {
	vector<int> cars;
	vector<int> requires;

	string input;
	getline(cin, input);
	stringstream ss(input);
	int n;
	
	while (ss >> n) {
		cars.emplace_back(n);
	}

	getline(cin, input);
	ss.clear();
	ss.str(input);

	while (ss >> n) {
		requires.emplace_back(n);
	}

	Solution sol;
	pair<int, int> res = sol.allocation(cars, requires);
	cout << res.first << " " << res.second << endl;
}