/*
 * 力扣 128 - 最长连续序列（Longest Consecutive Sequence）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给定一个未排序的整数数组 nums，找出数字连续的最长序列（不要求在原数组中连续）的长度。
 * 请你设计并实现时间复杂度为 O(n) 的算法解决此问题。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示数组长度。
 * - 第二行：n 个空格分隔的整数，表示数组 nums。
 *
 * 【输出格式】
 * - 一行：一个整数，表示最长连续序列的长度。
 *
 * 【样例输入】
 * 6
 * 100 4 200 1 3 2
 *
 * 【样例输出】
 * 4
 *
 * 【样例说明】
 * 连续序列为 [1, 2, 3, 4]，长度为 4。
 *
 * 【样例输入 2】
 * 10
 * 0 3 7 2 5 8 4 6 0 1
 *
 * 【样例输出 2】
 * 9
 *
 * （连续序列为 0,1,2,3,4,5,6,7,8，长度为 9。）
 *
 * 【数据范围】
 * 0 <= n <= 10^5
 * -10^9 <= nums[i] <= 10^9
 */

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;
class Solution {
public:
    int longest(vector<int>& nums) {
        int res = 0;
        unordered_set<int> mp;
        for(int& i : nums) {
            mp.insert(i);
        }
        for (int x : mp) {
            if (mp.count(x - 1)) continue;  // 只从连续段起点开始数，保证 O(n)
            int len = 1;
            while (mp.count(x + 1)) {
                x++;
                len++;
            }
            res = max(res, len);
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
    cout << sol.longest(nums) << endl;
}
