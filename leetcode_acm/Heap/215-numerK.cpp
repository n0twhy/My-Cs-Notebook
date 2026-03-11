/*
 * 力扣 215 - 数组中的第 K 个最大元素（Kth Largest Element in an Array）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
 * 注意：是排序后的第 k 个最大元素，不是第 k 个 distinct 元素。
 *
 * 【输入格式】
 * - 第一行：两个整数 n 和 k，表示数组长度和 k。
 * - 第二行：n 个空格分隔的整数，表示数组 nums。
 *
 * 【输出格式】
 * - 一行：一个整数，表示第 k 个最大的元素。
 *
 * 【样例输入】
 * 6 2
 * 3 2 1 5 6 4
 *
 * 【样例输出】
 * 5
 *
 * 【样例说明】
 * 排序后为 [1,2,3,4,5,6]，第 2 大的元素是 5。
 *
 * 【数据范围】
 * 1 <= k <= n <= 10^5
 * -10^4 <= nums[i] <= 10^4
 */

 #include <vector>
 #include <iostream>
using namespace std;
class Solution {
public:
    void heapMinify(vector<int>& nums, int i) {
        if(i >= nums.size()) {
            return;
        }
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int small = i;
        if(left < nums.size() && nums[small] > nums[left]) {
            small = left;
        }
        if(right < nums.size() && nums[small] > nums[right]) {
            small = right;
        }
        if(small == i) {
            return;
        }else {
            swap(nums[i], nums[small]);
            if(small == left) heapMinify(nums, left);
            if(small == right) heapMinify(nums, right);
        }
    }

    void heapBuild (vector<int>& nums) {
        int last = (nums.size() - 1) / 2;
        for(int i = last; i >= 0; --i) {
            heapMinify(nums, i);
        }
    }
    int NumberK(vector<int>& nums, int k) {
        vector<int> heap(k);
        for(int i = 0; i < k; ++i) {
            heap[i] = nums[i];
        }
        heapBuild(heap);
        for(int i = k; i < nums.size(); ++i) {
            if(nums[i] > heap[0]) {
                swap(nums[i], heap[0]);
                heapMinify(heap, 0);
            }
        }
        return heap[0];
    }

};

int main() {
    int n;
    int k;
    cin >> n;
    cin >> k;
    vector<int> nums(n);
    for(int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    Solution sol;
    cout << sol.NumberK(nums, k) << endl;
}