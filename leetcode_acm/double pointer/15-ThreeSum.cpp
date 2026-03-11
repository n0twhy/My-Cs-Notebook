/*
 * 力扣 15 - 三数之和（3Sum）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给你一个整数数组 nums，请你找出所有满足「三个数之和为 0」且不重复的三元组。
 * 三元组 [nums[i], nums[j], nums[k]] 需满足：i、j、k 两两不同，且 nums[i] + nums[j] + nums[k] == 0。
 * 答案中不得包含重复的三元组。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示数组长度。
 * - 第二行：n 个空格分隔的整数，表示数组 nums。
 *
 * 【输出格式】
 * - 第一行：一个整数 G，表示三元组的数量。
 * - 接下来 G 行：每行三个整数，表示一个三元组（按升序输出，如 -1 0 1），三元组之间顺序任意。
 *
 * 【样例输入】
 * 6
 * -1 0 1 2 -1 -4
 *
 * 【样例输出】
 * 2
 * -1 -1 2
 * -1 0 1
 *
 * 【数据范围】
 * 3 <= n <= 3000
 * -10^5 <= nums[i] <= 10^5
 */
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
class Solution {
public:
    vector<vector<int>> ThreeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        for(int i = 0; i < nums.size() - 2; ++i) {
            if (i > 0 && nums[i] == nums[i-1]) continue;
            int left = i + 1;
            int right = nums.size() - 1;
            while(left < right) {
                int sum = nums[left] + nums[right] + nums[i];
                if(sum == 0) {
                    vector<int> curr = {nums[i], nums[left], nums[right]};
                    res.push_back(curr);
                    while(left < right && nums[left] == nums[left + 1]) {
                        left++;
                    }
                    while(left < right && nums[right] == nums[right - 1]) {
                        right--;
                    }
                    left++;
                    right--;
                } else if(sum > 0) {
                    right--;
                } else {
                    left++;
                }
            }
        }
        return res;
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    Solution sol;
    vector<vector<int>> res = sol.ThreeSum(nums);
    cout << res.size() << endl;
    for(auto& v : res) {
        for(auto& i : v) {
            cout << i << " ";
        }    
        cout << endl;
    }
}