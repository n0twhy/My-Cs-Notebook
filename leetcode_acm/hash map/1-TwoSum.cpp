/*
 * 力扣 1 - 两数之和（Two Sum）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给定一个整数数组 nums 和一个整数 target，在数组中找出和为目标值 target 的那两个整数，
 * 并返回它们的数组下标（下标从 0 开始）。
 * 假设每种输入恰好对应一个答案，且同一个元素不能重复使用。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示数组长度。
 * - 第二行：n 个空格分隔的整数，表示数组 nums。
 * - 第三行：一个整数 target，表示目标值。
 *
 * 【输出格式】
 * - 一行：两个整数，表示满足条件的两个下标，用空格分隔（顺序任意）。
 *
 * 【样例输入】
 * 4
 * 2 7 11 15
 * 9
 *
 * 【样例输出】
 * 0 1
 *
 * 【样例说明】
 * nums[0] + nums[1] = 2 + 7 = 9，因此输出 0 1（或 1 0 均可）。
 *
 * 【数据范围】
 * 2 <= n <= 10^4
 * -10^9 <= nums[i] <= 10^9
 * -10^9 <= target <= 10^9
 * 保证有且仅有一组解。
 */
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;
class Solution {
public:
    pair<int, int> TwoSum (vector<int>& nums, int key) {
        unordered_map<int, int> mp;
        int n = nums.size();
        for(int i = 0; i < n; ++i) {
            mp[nums[i]] = i;
        }
        for(int i = 0; i < n; ++i) {
            if(mp.count(key - nums[i]) &&  mp[key - nums[i]] != i) {
                return {i, mp[key - nums[i]]}; 
            }
        }
    }
};

int main() {
    int n;
    int target;
    cin >> n;
    vector<int> nums(n);
    for(int i = 0; i < n; ++i) {
        int v;
        cin >> v;
        nums[i] = v;
    }
    cin >> target;
    Solution sol;
    auto res = sol.TwoSum(nums, target);
    cout << res.first << " " << res.second << endl;
}