/*
 * LeetCode 94 - 二叉树的中序遍历 (Binary Tree Inorder Traversal)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一棵二叉树的根节点，返回其节点值的中序遍历结果。
 * 中序遍历顺序：左子树 -> 根节点 -> 右子树。
 *
 * 【输入格式】
 * 第一行：一个整数 L，表示层序遍历序列的长度（含空位标记）。
 * 第二行：L 个由空格分隔的整数，表示二叉树的层序遍历序列。其中 999 表示空节点（null），
 *        其余整数为节点值。根节点为序列的第一个非空元素。
 *
 * 【输出格式】
 * 一行，中序遍历得到的节点值，相邻数字之间用一个空格分隔，行末可有空格。
 *
 * 【样例输入 1】
 * 7
 * 1 999 2 999 999 3 999
 *
 * 【样例输出 1】
 * 1 3 2
 *
 * （对应树：根 1，右子 2，2 的左子 3；中序：先 1，再 3，再 2）
 *
 * 【样例输入 2】
 * 1
 * 1
 *
 * 【样例输出 2】
 * 1
 *
 * 【样例输入 3】
 * 1
 * 999
 *
 * 【样例输出 3】
 *
 * （空树，输出空行或 nothing）
 *
 * 【数据范围】
 * 节点个数 [0, 100]
 * 节点值范围 [-100, 100]，且 999 仅用于表示空节点，不会作为真实节点值。
 *
 * =============================================================================
 */

#include <utility>
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
  void helper(vector<int>& res, TreeNode* root) {
    if(!root) return;
    helper(res, root->left);
    res.push_back(root->val);
    helper(res, root->right);
    
  }
  vector<int> inorderSearch(TreeNode* root) {
    vector<int> res;
    helper(res, root);
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
  queue<pair<Solution::TreeNode*, int>> q;
  Solution::TreeNode* root = new Solution::TreeNode(tree[0]);
  q.push({root, 0});
  
  while(!q.empty()) {
    Solution::TreeNode* curr = q.front().first;
    int p = q.front().second;
    q.pop();
    if((2*p + 1 < n) && tree[2*p+1] != 999) {
      curr->left = new Solution::TreeNode(tree[2*p+1]);
      q.push({curr->left, 2*p+1});
    }
    if((2*p + 2 < n) && tree[2*p+2] != 999) {
      curr->right = new Solution::TreeNode(tree[2*p+2]);
      q.push({curr->right, 2*p+2});
    }
  }

  Solution sol;
  vector<int> res = sol.inorderSearch(root);
  for(int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
  

}