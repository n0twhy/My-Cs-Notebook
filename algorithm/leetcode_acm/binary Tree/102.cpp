/*
 * LeetCode 102 - 二叉树的层序遍历 (Binary Tree Level Order Traversal)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一棵二叉树的根节点，返回其层序遍历结果（逐层从左到右输出节点值）。
 * 层序：第 0 层为根，第 1 层为根的左右子，第 2 层为再下一层……同一层内从左到右。
 *
 * 【输入格式】
 * 第一行：一个整数 L，表示层序遍历序列的长度（含空位标记）。
 * 第二行：L 个由空格分隔的整数，表示二叉树的层序遍历序列。其中 999 表示空节点（null），
 *        其余整数为节点值。根节点为序列的第一个非空元素。
 *
 * 【输出格式】
 * 第一行：一个整数 K，表示树的层数（根所在层为第 0 层）。
 * 接下来 K 行：第 i 行（i 从 0 开始）为第 i 层的节点值，从左到右，相邻数字之间用一个空格分隔，行末可有空格。
 *
 * 【样例输入 1】
 * 7
 * 1 999 2 999 999 3 999
 *
 * 【样例输出 1】
 * 3
 * 1
 * 2
 * 3
 *
 * （对应树：根 1，右子 2，2 的左子 3；第 0 层 1，第 1 层 2，第 2 层 3，共 3 层）
 *
 * 【样例输入 2】
 * 7
 * 3 9 20 999 999 15 7
 *
 * 【样例输出 2】
 * 3
 * 3
 * 9 20
 * 15 7
 *
 * 【样例输入 3】
 * 1
 * 1
 *
 * 【样例输出 3】
 * 1
 * 1
 *
 * 【样例输入 4】
 * 1
 * 999
 *
 * 【样例输出 4】
 * 0
 *
 * （空树：层数 K=0，无后续行）
 *
 * 【数据范围】
 * 节点个数 [0, 100]
 * 节点值范围 [-100, 100]，且 999 仅用于表示空节点，不会作为真实节点值。
 *
 * =============================================================================
 */

#include <vector>
#include <iostream>
#include <queue>

using namespace std;
class Solution {
public:
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {} 
  TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};
  vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if (!root) return res;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
      int sz = q.size();
      vector<int> row;
      for (int i = 0; i < sz; ++i) {
        TreeNode* curr = q.front();
        q.pop();
        row.push_back(curr->val);
        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
      }
      res.push_back(row);
    }
    return res;
  }
};

int main() {
  int n;
  cin >> n;
  vector<int> tree(n);

  for(int i = 0; i < n; ++i) {
    cin >> tree[i];
  }
  Solution::TreeNode* root = new Solution::TreeNode(tree[0]);
  queue<pair<Solution::TreeNode*, int>> q;
  q.push({root, 0});
  while(!q.empty()) {
    Solution::TreeNode* curr = q.front().first;
    int p = q.front().second;
    q.pop();
    int left = 2 * p + 1;
    int right = 2 * p + 2;
    if (left < n && tree[left] != 999) {
      curr->left = new Solution::TreeNode(tree[left]);
      q.push({curr->left, left});
    }
    if (right < n && tree[right] != 999) {
      curr->right = new Solution::TreeNode(tree[right]);
      q.push({curr->right, right});
    }
  }
  Solution sol;
  vector<vector<int>> res = sol.levelOrder(root);
  cout << res.size() << endl;
  for(int i = 0; i < res.size(); ++i) {
    for(int j = 0; j < res[i].size(); ++j) {
      cout << res[i][j] << " ";
    }
    cout << endl;
  }
}