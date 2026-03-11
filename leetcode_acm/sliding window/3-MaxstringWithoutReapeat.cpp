/*
 * 力扣 3 - 无重复字符的最长子串（Longest Substring Without Repeating Characters）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给定一个字符串 s，请你找出其中不含有重复字符的「最长子串」的长度。
 *
 * 【输入格式】
 * - 一行：字符串 s，仅由英文字母、数字、符号和空格组成。
 *
 * 【输出格式】
 * - 一行：一个整数，表示无重复字符的最长子串的长度。
 *
 * 【样例输入】
 * abcabcbb
 *
 * 【样例输出】
 * 3
 *
 * 【样例说明】
 * 最长无重复子串为 "abc"，长度为 3。
 *
 * 【样例输入 2】
 * bbbbb
 *
 * 【样例输出 2】
 * 1
 *
 * 【样例输入 3】
 * pwwkew
 *
 * 【样例输出 3】
 * 3
 *
 * （最长子串为 "wke" 或 "kew"，长度 3。）
 *
 * 【数据范围】
 * 0 <= s.length <= 5 * 10^4
 * s 由英文字母、数字、符号和空格组成
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;
class Solution {
public:
    int longestSub(string& s) {
        unordered_map<char, int> last;
        int left = 0;
        int right = 0;
        int res = 1;
        while(right < s.size()) {
            if(last[s[right]] >= left ) {
                res = max(res, right - left);
                left = last[s[right]] + 1;            
            }
            last[s[right]] = right;
            right++;
        }
        return res;
    }
};

int main() {
    string s;
    cin >> s;
    Solution sol;
    int res = sol.longestSub(s);
    cout << res << endl;
}