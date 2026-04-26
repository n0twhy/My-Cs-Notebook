/*
力扣 98. 验证二叉搜索树
https://leetcode.cn/problems/validate-binary-search-tree/

题目描述：
给你一个二叉树的根节点 root，判断其是否是一个有效的二叉搜索树。

有效二叉搜索树定义如下：
- 节点的左子树只包含小于当前节点的数。
- 节点的右子树只包含大于当前节点的数。
- 所有左子树和右子树自身必须也是二叉搜索树。

输入描述：
输入一行，表示二叉树的层序序列化数组。
节点值之间用空格分隔，空节点用 null 表示。

输出描述：
若输入是一棵有效的二叉搜索树，输出 true；否则输出 false。

样例 1：
输入：
2 1 3
输出：
true

样例 2：
输入：
5 1 4 null null 3 6
输出：
false
说明：
根节点的值是 5，但是其右子节点的值是 4，不满足二叉搜索树定义。

提示：
- 树中节点数目范围在 [1, 10^4] 内
- -2^31 <= Node.val <= 2^31 - 1
*/
#include <bits/stdc++.h>
#include <climits>
using namespace std;
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;

  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

TreeNode *BuildTree(vector<string>& tokens) {
  if (tokens.empty()) return nullptr;

  TreeNode *root = new TreeNode(stoi(tokens[0]));
  queue<TreeNode *> q;
  q.push(root);
  int i = 0;
  while (i < tokens.size()) {
    auto curr = q.front();
    q.pop();

    if (2 * i + 1 < tokens.size() && tokens[2 * i + 1] != "null") {
      curr->left = new TreeNode(stoi(tokens[2 * i + 1]));
      q.push(curr->left);
    }
    i++;
    if (2 * i + 2 < tokens.size() && tokens[2 * i + 2] != "null") {
      curr->right = new TreeNode(stoi(tokens[2 * i + 2]));
      q.push(curr->right);
    }
    i++;
  }
  return root;
}

class Solution {
public:
  bool dfs(TreeNode *root, int min, int max) {
    if (root == nullptr) return true; 
    if (root->val <= min || root->val >= max) return false;
    return dfs(root->left, min, root->val) && dfs(root->right, root->val, max);
  }
  
};

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  string token;
  vector<string> tokens;
  while (ss >> token) {
    tokens.emplace_back(token);
  }

  Solution sol;
  bool res = sol.dfs(BuildTree(tokens), INT_MIN, INT_MAX);
  if (res) cout << "true" << endl;
  else cout << "false" << endl;
}