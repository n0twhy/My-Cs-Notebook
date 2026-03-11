/*
题目描述：
给定一个由字符 '0' 和 '1' 组成的二维网格，'1' 表示陆地，'0' 表示水域。
如果两个陆地格子在上下左右四个方向之一相邻，则认为它们属于同一座岛屿。
请你计算并输出网格中岛屿的数量。

输入格式：
- 第一行包含两个整数 m, n（表示网格的行数和列数）。
- 接下来有 m 行，每行是一个长度为 n 的字符串，仅由字符 '0' 和 '1' 组成：
  - 第 i 行表示网格的第 i 行。
  - 其中第 j 个字符表示坐标为 (i, j) 的格子。

输出格式：
- 输出一行，包含一个整数，表示网格中岛屿的数量。

样例输入 1：
4 5
11110
11010
11000
00000

样例输出 1：
1

样例输入 2：
4 5
11000
11000
00100
00011

样例输出 2：
3

数据范围：
- 1 <= m, n <= 300
- 输入的每行字符串长度恰好为 n
- 字符仅为 '0' 或 '1'
- 保证输入格式合法，无需额外校验
*/
#include <queue>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    int islandCount(vector<vector<int>>& island) {
        int m = island.size();
        int n = island[0].size();
        int count = 0;
        for(int i = 0; i < m; ++i) {
            for(int j = 0; j < n; ++j) {
                if(island[i][j] == 1) {
                    count++;
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    island[i][j] = 0;
                    while(!q.empty()) {
                        int x = q.front().first;
                        int y = q.front().second;
                        q.pop();
                        for(int k = 0; k < 4; ++k) {
                            int a = x + dx[k];
                            int b = y + dy[k];
                            if(a >= 0 && b >= 0 && a < m && b < n && island[a][b] == 1) {
                                q.push({a, b});
                                island[a][b] = 0;
                            }
                        }
                    }
                }
            }
        }
        return count;
    }
};

int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<int>> island(m, vector<int>(n));
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            cin >> island[i][j];
        }
    }
    Solution sol;
    int res = sol.islandCount(island);
    cout << res << endl;
    
}