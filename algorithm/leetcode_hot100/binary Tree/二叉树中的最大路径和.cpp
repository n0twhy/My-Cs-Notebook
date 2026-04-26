/*
力扣 124. 二叉树中的最大路径和
https://leetcode.cn/problems/binary-tree-maximum-path-sum/

题目描述：
二叉树中的路径被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。
同一个节点在一条路径序列中至多出现一次。该路径至少包含一个节点，且不一定经过根节点。

路径和是路径中各节点值的总和。

给你一个二叉树的根节点 root，返回其最大路径和。

输入描述：
输入一行，表示二叉树的层序序列化数组，节点值之间用空格分隔，空节点用 null 表示。

输出描述：
输出一个整数，表示最大路径和。

样例 1：
输入：
1 2 3
输出：
6
说明：
最优路径是 2 -> 1 -> 3，路径和为 2 + 1 + 3 = 6。

样例 2：
输入：
-10 9 20 null null 15 7
输出：
42
说明：
最优路径是 15 -> 20 -> 7，路径和为 15 + 20 + 7 = 42。

提示：
- 树中节点数目范围是 [1, 3 * 10^4]
- -1000 <= Node.val <= 1000
*/

#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;

  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

TreeNode* buildTree(const vector<string>& tokens) {
  if (tokens.empty() || tokens[0] == "null")
      return nullptr;

  TreeNode* root = new TreeNode(stoi(tokens[0]));
  queue<TreeNode*> q;
  q.push(root);
  int i = 1;
  while (i < tokens.size()) {
      TreeNode* current = q.front();
      q.pop();
      // 构造左子节点
      if (i < tokens.size() && tokens[i] != "null") {
          current->left = new TreeNode(stoi(tokens[i]));
          q.push(current->left);
      }
      i++;
      // 构造右子节点
      if (i < tokens.size() && tokens[i] != "null") {
          current->right = new TreeNode(stoi(tokens[i]));
          q.push(current->right);
      }
      i++;
  }
  return root;
}

class Solution {
public:
  int dfs(TreeNode *root) {
    if (root == nullptr) return 0;
    int contribution;
    int left_cont = max(dfs(root->left), 0);
    int right_cont = max(dfs(root->right), 0);
    contribution = max(left_cont, right_cont);
    maxsum = max(left_cont + right_cont + root->val, maxsum);
    return contribution + root->val;
  }
  int maxpathsum(TreeNode *root) {
    maxsum = root->val;
    dfs(root);
    return maxsum;
  }
private:
  int maxsum;

};

int main() {
  queue<pair<TreeNode *, int>> q;
  string input;
  getline(cin, input);
  string n;
  vector<string> tree;
  stringstream ss(input);
  while (ss >> n) {
    tree.emplace_back(n);
  }
  
  Solution sol;
  cout << sol.maxpathsum(buildTree(tree)) << endl;
  
}