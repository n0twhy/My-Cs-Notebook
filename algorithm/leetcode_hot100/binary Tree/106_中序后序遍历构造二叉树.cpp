/*
 * LeetCode 106 - 从中序与后序遍历序列构造二叉树 (Construct Binary Tree from Inorder and Postorder Traversal)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给定两个整数数组 inorder 和 postorder，其中 inorder 是二叉树的中序遍历，postorder 是同一棵树的后序遍历。
 * 请构造并返回这棵二叉树。输出时需将构造出的树按层序遍历序列打印（含空位标记，便于判题）。
 *
 * 【输入格式】
 * 第一行：一个整数 n，表示树中节点个数。
 * 第二行：n 个由空格分隔的整数，表示中序遍历序列（inorder）。
 * 第三行：n 个由空格分隔的整数，表示后序遍历序列（postorder）。
 *
 * 【输出格式】
 * 第一行：一个整数 L，表示层序遍历序列的长度（含空位标记）。
 * 第二行：L 个由空格分隔的整数，表示构造出的二叉树的层序遍历序列。空节点用 999 表示。
 * （层序规则：下标 p 的节点，左子下标 2p+1，右子下标 2p+2；不足位置用 999 补齐。）
 *
 * 【样例输入 1】
 * 5
 * 9 3 15 20 7
 * 9 15 7 20 3
 *
 * 【样例输出 1】
 * 7
 * 3 9 20 999 999 15 7
 *
 * （树：根 3，左子 9，右子 20；20 的左子 15，右子 7）
 *
 * 【样例输入 2】
 * 2
 * 2 1
 * 2 1
 *
 * 【样例输出 2】
 * 3
 * 1 2 999
 *
 * （根 1，左子 2，无右子）
 *
 * 【样例输入 3】
 * 1
 * -1
 * -1
 *
 * 【样例输出 3】
 * 1
 * -1
 *
 * 【数据范围】
 * 1 <= n <= 3000
 * inorder.length == postorder.length == n
 * inorder 和 postorder 均由互不相同的整数组成
 * postorder 中最后一个元素为整棵树的根节点值
 * inorder 与 postorder 均保证对应同一棵有效的二叉树
 * 节点值范围 [-3000, 3000]，输出时空节点用 999 表示（保证真实节点值不为 999）
 *
 * =============================================================================
 */
#include <cstddef>
#include <iterator>
#include <queue>
#include <vector>
#include <iostream>
#include <unordered_map>

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

  TreeNode* helper(vector<int>& inorder, vector<int>& postorder, int midleft, int midright, int postleft, int postright) {
    if(midleft > midright || postleft > postright) {
      return nullptr;
    }
    int i = inMap[postorder[postright]];
    TreeNode* root = new TreeNode(postorder[postright]);
    int leftChildrenSize = i - midleft;
    root->left = helper(inorder, postorder, midleft, i - 1, postleft, postleft + leftChildrenSize - 1);
    root->right = helper(inorder, postorder, i + 1, midright, postleft + leftChildrenSize, postright - 1);
    return root;
  }

  TreeNode* TreeNodeBuilding(vector<int>& inorder, vector<int>& postorder) {
    for(int i = 0; i < inorder.size(); ++i) {
      inMap[inorder[i]] = i;
      postMap[postorder[i]] = i;
    }

    return helper(inorder, postorder, 0, inorder.size() - 1, 0, postorder.size() - 1);
  }
private:
  unordered_map<int, int> inMap;
  unordered_map<int, int> postMap;
};

int main() {
  int n;
  cin >> n;
  vector<int> inorder(n);
  vector<int> postorder(n);
  for(int i = 0; i < n; ++i) {
    cin >> inorder[i];
  }
  for(int i = 0; i < n; ++i) {
    cin >> postorder[i];
  }
  Solution sol;
  Solution::TreeNode* root = sol.TreeNodeBuilding(inorder, postorder);
  if (!root) {
    cout << "0\n";
    return 0;
  }
  queue<pair<Solution::TreeNode*, int>> q;
  q.push({root, 0});
  unordered_map<int, int> mp;
  int maxIndex = 0;
  while (!q.empty()) {
    Solution::TreeNode* curr = q.front().first;
    int i = q.front().second;
    q.pop();
    mp[i] = curr->val;
    if (curr->left) {
      q.push({curr->left, 2 * i + 1});
      maxIndex = max(maxIndex, 2 * i + 1);
    }
    if (curr->right) {
      q.push({curr->right, 2 * i + 2});
      maxIndex = max(maxIndex, 2 * i + 2);
    }
  }
  int L = maxIndex + 1;
  cout << L << endl;
  for (int i = 0; i < L; ++i) {
    if (i > 0) cout << " ";
    cout << (mp.count(i) ? mp[i] : 999);
  }
  cout << endl;
  return 0;
}