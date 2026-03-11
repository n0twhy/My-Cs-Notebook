/*
 * 力扣 11 - 盛最多水的容器（Container With Most Water）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给定一个长度为 n 的整数数组 height。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i])。
 * 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
 * 返回容器可以储存的最大水量（面积）。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示数组长度。
 * - 第二行：n 个空格分隔的非负整数，表示每条垂线的高度 height[i]。
 *
 * 【输出格式】
 * - 一行：一个整数，表示能盛放的最大水量（面积）。
 *
 * 【样例输入】
 * 9
 * 1 8 6 2 5 4 8 3 7
 *
 * 【样例输出】
 * 49
 *
 * 【样例说明】
 * 下标 1 与 8 的线（高度 8 和 7）形成的容器：宽度 7，高度取 min(8,7)=7，面积 7*7=49。
 *
 * 【数据范围】
 * 2 <= n <= 10^5
 * 0 <= height[i] <= 10^4
 */
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
class Solution {
public:
    int Container(vector<int>& height) {
        int left = 0;
        int right = height.size() - 1;
        int maxsize = 0;
        while(left < right) {
            maxsize = max((right - left) * min(height[left], height[right]), maxsize);
            if(height[left] < height[right]) {
                left++;
            }else {
                right--;
            }
        }
        return maxsize;
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> height(n);
    for(int i = 0; i < n; ++i) {
        cin >> height[i];
    }
    Solution sol;
    int res = sol.Container(height);
    cout << res << endl;
    
}