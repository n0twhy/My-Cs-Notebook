/*
题目：建设基站

题目内容
有一棵二叉树，每个节点上都住了一户居民。
现在要给这棵树上的居民建设基站，每个基站只能覆盖其所在节点及相邻节点，
请问信号覆盖这棵树最少需要建设多少个基站。

输入描述
输入为一个整数数组 nums（1 <= nums.length <= 3000），元素之间用空格分隔，
表示二叉树的层序节点值。

其中：
- 正整数表示存在节点；
- N 表示不存在节点。

例如：
[1 2 3 4 N 5 6]
表示一棵二叉树，最少需要建设 2 个基站。

输出描述
输出最少需要建设的基站个数。

样例1
输入
1 2 3 4 N 5 6

输出
2

说明
如图，2 个基站可以覆盖所有节点。

样例2
输入
1 2 N 3 N N 4

输出
2

说明
如图，2 个基站可以覆盖所有节点（左右两种方案都能覆盖所有节点）。
*/

#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

const int INF = 0x3f3f3f3f;

struct TreeNode {
  int val;
  TreeNode *left {nullptr};
  TreeNode *right {nullptr};
  TreeNode(int x) : val(x) {}
};

TreeNode *BuildTree(vector<string> &nums) {
  if (nums.empty() || nums[0] == "N") return nullptr;
  
  auto root = new TreeNode(stoi(nums[0]));
  queue<TreeNode *> q;
  q.push(root);
  
  int i = 1;  // 数组中下一个待消费的位置
  while (!q.empty() && i < (int)nums.size()) {
      TreeNode *node = q.front();
      q.pop();
      
      // 处理左孩子
      if (i < (int)nums.size()) {
          if (nums[i] != "N") {
              node->left = new TreeNode(stoi(nums[i]));
              q.push(node->left);
          }
          i++;
      }
      // 处理右孩子
      if (i < (int)nums.size()) {
          if (nums[i] != "N") {
              node->right = new TreeNode(stoi(nums[i]));
              q.push(node->right);
          }
          i++;
      }
  }
  return root;
}

class Solution {
public:
  vector<int> dfs(TreeNode *root) {
    if (root == nullptr) {
      return {INF, 0, 0};
    }

    vector<int> leftDP = dfs(root->left);
    vector<int> rightDP = dfs(root->right);

    vector<int> res(3);

    int leftMin = min({leftDP[0], leftDP[1], leftDP[2]});
    int rightMin = min({rightDP[0], rightDP[1], rightDP[2]});
    res[0] = 1 + leftMin + rightMin;

    int caseB1 = leftDP[0] + min(rightDP[0], rightDP[1]);
    int caseB2 = rightDP[0] + min(leftDP[0], leftDP[1]);
    res[1] = min(caseB1, caseB2);

    res[2] = leftDP[1] + rightDP[1];
    return res;
  }

  int minimal(vector<string> &nums) {
    auto root = BuildTree(nums);
    return min(dfs(root)[0],dfs(root)[1]);
  }
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  string num;
  vector<string> nums;
  while (ss >> num) {
    nums.emplace_back(num);
  }
  Solution sol;
  cout << sol.minimal(nums) << endl;
}
