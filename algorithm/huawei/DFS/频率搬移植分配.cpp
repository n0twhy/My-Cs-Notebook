/*
题目：频率搬移植分配

题目内容
在无线通信设备中通常使用超外差接收机。信号通过混频器后，频率会搬移一个数值。
为节省器件，混频器尽可能共享，设计为二叉树型混频器组，可同时把信号搬移到不同频率。

二叉树为完全二叉树。给定二叉树层数（等价给定叶子节点数）和从根节点到各叶子节点的目标频率值，
要求输出该二叉树各节点的值（数组层序表示）。

规则
某节点值定义为：
该节点子树内所有叶子节点目标频率的「最大值与最小值的平均值（非整数向下取整）」
减去「该节点所有父节点值之和」。

示意：
若某节点子树叶子频率范围为 [10, 70]，则中值为 (10+70)/2 = 40。
若其父节点值总和为 30，则该节点值 = 40 - 30 = 10。

输入描述
第一行：叶子节点数 L（L 必定是 2^n），1 <= L <= 4096。
第二行：L 个叶子节点的目标频率值（0 <= 值 <= 1000000）。

输出描述
输出整棵完全二叉树的节点值，按数组形式（层序，从上到下、从左到右）输出。

样例
输入：
4
18 24 2 3

输出：
13 8 -11 -3 3 0 1

解释（按层）：
第0层根节点：
叶子范围 [2,24]，中值 (2+24)/2=13，父和为0，节点值=13。

第1层左节点：
叶子范围 [18,24]，中值 21，父和=13，节点值=8。
第1层右节点：
叶子范围 [2,3]，中值 2，父和=13，节点值=-11。

第2层（叶子层）：
节点1：中值18，父和13+8=21，值=-3
节点2：中值24，父和13+8=21，值=3
节点3：中值2，父和13+(-11)=2，值=0
节点4：中值3，父和13+(-11)=2，值=1
*/

#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  pair<int, int> FindMinAndMax(vector<int> &nums, int start, int end) {
    int min_num = nums[start];
    int max_mun = nums[start];
    for (int i = start; i <= end; ++i) {
      min_num = min(min_num, nums[i]);
      max_mun = max(max_mun, nums[i]);
    }

    return {min_num, max_mun};
  }
  void dfs(int start, int end, int index, int parent_sum) {
    auto [min_num, max_num] = FindMinAndMax(values_, start, end);
    int curr_value = (min_num + max_num) / 2 - parent_sum;
    int mid = (start + end) / 2;
    process_[index] = curr_value;
    
    
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (right < 2 * values_.size() - 1) {
      dfs(start, mid, left, parent_sum + curr_value);
      dfs(mid + 1, end, right, parent_sum + curr_value);
    } 
  }
  vector<int> Frequency(vector<int> &values) {
    process_.resize(2 * values.size() - 1);
    values_ = values;
    dfs(0, values_.size() - 1, 0, 0);
    return process_;
  }
private:
  vector<int> process_;
  vector<int> values_;
};

int main() {
    int n;
    cin >> n;
    vector<int> values(n);
    for (int i = 0; i < n; ++i) {
      cin >> values[i];
    }
    Solution sol;
    vector<int> res = sol.Frequency(values);
    for (int i = 0; i < res.size(); ++i) {
      cout << res[i] << " ";
    }
}
