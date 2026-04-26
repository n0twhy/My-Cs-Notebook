/*
题目内容
假设我们有一系列测试用例，每个测试用例会覆盖测试若干个代码模块。

我们用一个二维数组 cases 来表示这些测试用例的覆盖情况，
其中 cases[i][j] 为 1 表示第 i 个测试用例覆盖了第 j 个模块，为 0 则表示未覆盖。

求一个最小的测试用例集合，使得该集合能够覆盖所有代码模块。
返回最小集合的大小，如果不存在能够覆盖所有代码模块的测试用例集合，则返回 -1。

输入描述
第一行输入是两个整数，分别代表用例总数 i 和代码模块总数 j。

从第二行开始的 i 行，每一行有 j 个整数（0 或 1），
每个整数之间用空格分隔；每一行代表一个用例对代码模块的覆盖情况。

参数取值范围
cases[i].length = j
cases[i][j] = 0 或 1
1 <= i <= 20
1 <= j <= 20

输出描述
覆盖所有代码模块使用的最小用例集合的大小（int），
如果不存在能够覆盖所有模块的测试用例集合则返回 -1。

样例1
输入
3 2
1 0
1 0
1 0

输出
-1

说明
该输入代表有 3 个测试用例，有 2 个代码模块，
第一个测试用例 [1,0] 可以覆盖第 1 个代码模块，
第 2、3 个测试用例相同。
该输入不存在用例集合可以覆盖所有的程序，所以返回 -1。

样例2
输入
4 4
1 0 1 0
0 1 0 1
1 1 0 0
0 0 1 1

输出
2

说明
输入代表有 4 个测试用例，有 4 个代码模块。
针对该输入，可以使用用例 cases[0] 和用例 cases[1] 覆盖所有模块，
也可以选择使用用例 cases[2] 和用例 cases[3] 覆盖所有模块，
均满足最小用例数要求，所以返回 2。

样例3
输入
3 2
1 0
0 1
1 1

输出
1

说明
输入代表有 3 个测试用例，有 2 个代码模块。
针对该输入，可以使用用例 cases[2] 即可覆盖所有模块，
满足最小用例数要求，所以返回 1。
*/

#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstddef>
#include <functional>
#include <numeric>
using namespace std;

class Solution {
public:
  void dfs(vector<int> &binarys, int idx, int curr, int cnt) {
    if (curr == mask_) {
      min_cnt = min(min_cnt, cnt);
      return;
    }
    if (idx == binarys.size()) return;
    
    
    for (int i = idx; i < binarys.size(); ++i) {
      int old = curr;
      curr = (curr | binarys[i]);
      dfs(binarys, i + 1, curr, cnt + 1);
      curr = old;
    }
  }
  int TestsCover(vector<vector<int>> &instances) {
    test_cnt_ = instances[0].size();
    mask_ = (1 << test_cnt_) - 1;
    vector<int> binarys;
    for (auto &v : instances) {
      int curr = 0;
      for (int i = v.size() - 1; i >= 0; --i) {
        curr = v[i] | curr << 1;
      }
      binarys.emplace_back(curr);
    }

    dfs(binarys, 0, 0, 0);
    return min_cnt == INT_MAX ? -1 : min_cnt;
  }
private:
  size_t test_cnt_;
  int mask_;
  int min_cnt {INT_MAX};
};

int main() {
  int m, n;
  cin >> m >> n;
  vector<vector<int>> input(m, vector<int>(n));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> input[i][j];
    }
  }
  Solution sol;
  cout << sol.TestsCover(input) << endl;
}