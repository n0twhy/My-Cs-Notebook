/*
力扣 46. 全排列
https://leetcode.cn/problems/permutations/

题目描述：
给定一个不含重复数字的数组 nums，返回其所有可能的全排列。你可以按任意顺序返回答案。

示例 1：
输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

示例 2：
输入：nums = [0,1]
输出：[[0,1],[1,0]]

示例 3：
输入：nums = [1]
输出：[[1]]

提示：
- 1 <= nums.length <= 6
- -10 <= nums[i] <= 10
- nums 中的所有整数互不相同
*/
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  void helper(vector<int> &nums, vector<int> &curr_nums) {
    if (curr_nums.size() == nums.size()) {
      ans.emplace_back(curr_nums);
    }
    for (int i = 0; i < nums.size(); ++i) {
      if (mark[i]) continue;
      curr_nums.emplace_back(nums[i]);
      mark[i] = true;
      helper(nums, curr_nums);
      mark[i] = false;
      curr_nums.pop_back();
    }
  }
  vector<vector<int>> fullarray(vector<int> &nums) {
    mark.resize(nums.size(), false);
    vector<int> curr_nums;
    helper(nums, curr_nums);
    return ans;
  }
private:
  vector<vector<int>> ans;
  vector<bool> mark;
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  vector<int> nums;
  int n;
  while (ss >> n) {
    nums.emplace_back(n);
  }

  Solution sol;
  vector<vector<int>> ans = sol.fullarray(nums);

  for (int i = 0; i < ans.size(); ++i) {
    for (int j = 0; j < ans[i].size(); ++j) {
      cout << ans[i][j] << " ";
    }
    cout << endl;
  }
}