/*
 * 力扣 49 - 字母异位词分组（Group Anagrams）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给你一个字符串数组，请你将「字母异位词」组合在一起。可以按任意顺序返回结果。
 * 字母异位词：由重新排列同一单词的字母得到，且每个字母恰好使用一次。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示字符串个数。
 * - 接下来 n 行：每行一个只由小写英文字母组成的字符串。
 *
 * 【输出格式】
 * - 第一行：一个整数 G，表示分组的数量。
 * - 接下来 G 行：每行表示一组字母异位词，组内字符串用空格分隔（组与组、组内顺序均任意）。
 *
 * 【样例输入】
 * 6
 * eat
 * tea
 * tan
 * ate
 * nat
 * bat
 *
 * 【样例输出】
 * 3
 * bat
 * nat tan
 * ate eat tea
 *
 * （或任意一种分组正确、格式符合的答案，例如先输出 ate eat tea 再 bat 再 nat tan 等。）
 *
 * 【数据范围】
 * 1 <= n <= 10^4
 * 0 <= strs[i].length <= 100
 * strs[i] 仅由小写英文字母组成
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
class Solution {
public:
    vector<vector<string>> Anagrams(vector<string>& words) {
        unordered_map<string, vector<string>> mp;
        for(auto& s : words) {
            vector<int> count(26, 0);
            for(char& c : s) {
                count[c - 'a']++;
            }
            string curr;
            for(int i = 0; i < 26; ++i) {
                curr.push_back('#');
                curr += to_string(count[i]);
            }
            mp[curr].push_back(s);
        }
        vector<vector<string>> res;
        for(auto& i : mp) {
            res.push_back(i.second);
        }
        return res;
    }
};

int main() {
    int n;
    cin >> n;
    vector<string> words(n);
    for(int i = 0; i < n; ++i) {
        string curr;
        cin >> curr;
        words[i] = curr;
    }
    Solution sol;
    vector<vector<string>> res = sol.Anagrams(words);
    cout << res.size() << endl;
    for(auto& v : res) {
        for(auto i : v) {
            cout << i << " ";
        }
        cout << endl;
    }
    
}