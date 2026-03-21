/*
 * LeetCode 236 - 二叉树的最近公共祖先 (Lowest Common Ancestor of a Binary Tree)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定一棵二叉树的根节点，以及树中两个节点的值 p 和 q，请返回它们的最近公共祖先（LCA）的节点值。
 * 最近公共祖先定义：节点 T 为 p、q 的最近公共祖先，当且仅当 T 是 p、q 的公共祖先，且 T 的深度尽可能大
 * （一个节点也可以是它自己的祖先）。
 *
 * 【输入格式】
 * 第一行：一个整数 L，表示层序遍历序列的长度（含空位标记）。
 * 第二行：L 个由空格分隔的整数，表示二叉树的层序遍历序列。其中 999 表示空节点（null），
 *        其余整数为节点值，且树中节点值互不相同。
 * 第三行：两个整数 p、q，表示要查询 LCA 的两个节点的值（保证 p、q 一定在树中出现）。
 *
 * 【输出格式】
 * 一行一个整数，表示 p 与 q 的最近公共祖先节点的值。
 *
 * 【样例输入 1】
 * 11
 * 3 5 1 6 2 0 8 999 999 7 4
 * 5 1
 *
 * （树：根 3，左 5 右 1；5 左 6 右 2；1 左 0 右 8；2 左 7 右 4；p=5, q=1，LCA 为 3）
 *
 * 【样例输出 1】
 * 3
 *
 * 【样例输入 2】
 * 11
 * 3 5 1 6 2 0 8 999 999 7 4
 * 5 4
 *
 * （同上树，p=5, q=4，LCA 为 5）
 *
 * 【样例输出 2】
 * 5
 *
 * 【样例输入 3】
 * 3
 * 1 2 3
 * 2
 * 3
 *
 * 【样例输出 3】
 * 1
 *
 * 【数据范围】
 * 节点个数 [2, 10^5]
 * 节点值范围 [-10^9, 10^9]，互不相同；999 仅用于表示空节点。
 * p、q 一定在树中存在。
 *
 * =============================================================================
 */
#include <iostream>
#include <queue>
#include <ratio>
#include <vector>
using  namespace std;
class Solution {
public:
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;

  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

bool helper(TreeNode* root) {
  if (!root) return false;
  if (root->val == num_1_) {
    res = num_1_;
    return true;
  }
  if (root->val == num_2_) {
    res = num_2_;
    return true;
  }
  bool left = helper(root->left);
  bool right = helper(root->right);
  if(left && right) {
    res = root->val;
  }
  return true;
}

int find(TreeNode* root, int num_1, int num_2) {
  num_1_ = num_1;
  num_2_ = num_2;
  helper(root);
  return res;
}

private:
int num_1_;
int num_2_;
int res;
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
    int i = q.front().second;
    Solution::TreeNode* curr = q.front().first;
    q.pop();
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && tree[left] != 999) {
      curr->left = new Solution::TreeNode(tree[left]);
      q.push({curr->left, left});
    }
    if (right < n && tree[right] != 999) {
      curr->right = new Solution::TreeNode(tree[right]);
      q.push({curr->right, right});
    }
  }
  int num_1, num_2;
  cin >> num_1 >> num_2;
  Solution sol;
  cout << sol.find(root, num_1, num_2) << endl;
}