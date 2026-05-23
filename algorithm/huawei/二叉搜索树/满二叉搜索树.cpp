/*
题目描述
小明精通二分查找，但是对二叉搜索树却一窍不通。现在小明手里有一棵平衡的满二叉搜索树。
由于小明对二叉搜索树这个概念不是很懂，他做了一些笔记如下：

（1）节点的左子树只包含小于当前节点的数。
（2）节点的右子树只包含大于当前节点的数。
（3）所有左子树和右子树自身必须也是二叉搜索树。

为了更好地了解这个数据结构的功能，现在小明给你一个待查整数，
请你输出查找路径以及查询结果。

输入描述
第一行输入 2^n-1 个整数，表示整棵满二叉搜索树。
其中 1<=n<=10，整数之间用空格分割。

第二行输入一个待查找的整数。

规定：所有整数 num∈[-32768,32767]。

输出描述
输出一个字符串，表示搜索路径以及结果。

规定：
搜索路径起点为根节点，用 S 表示；
查找右子树用 R 表示；
查找左子树用 L 表示；
找到对应整数后用 Y 表示；
若最终未找到则用 N 表示。

样例一
输入
2 1 3 7 5 6 4
6

输出
SRY

解释
从根节点开始，所以路径第一部分为 S；
待查找数 6 大于根节点 4，所以查找右子树，路径增加 R；
正好找到目标值，因此最后增加 Y，最终输出 SRY。

样例二
输入
4 2 1 3 6 5 7
5

输出
SRLY

解释
从根节点开始，先往右子树，再往左子树查找，找到结果 5，
因此最终输出 SRLY。

样例三
输入
1 2 3 4 5 6 7
8

输出
SRRN

解释
从根节点开始查找，先标记 S；
待查找数 8 比 4 大，查找右子树，标记 R；
8 比 6 还大，继续查找右子树，标记 R；
8 比节点 7 还大，但已经到达叶子节点仍未找到，因此标记 N，
最终输出 SRRN。
*/
#include <bits/stdc++.h>
#include <cmath>
#include <vector>
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left{nullptr};
  TreeNode *right{nullptr};

  TreeNode(int v) : val(v) {}
};
/*
TreeNode *BuildTree(vector<int> &tree) {
  if (tree.size() == 0) return nullptr;
  queue<TreeNode *> q;
  TreeNode *root = new TreeNode(tree[0]);
  int i = 1;
  q.push(root);
  while (!q.empty()) {
    TreeNode *curr = q.front();
    q.pop();

    if (i < tree.size() && tree[i]) {
      curr->left = new TreeNode(tree[i]);
      q.push(curr->left);
    }
    i++;
    if (i < tree.size()) {
      curr->right = new TreeNode(tree[i]);
      q.push(curr->right);
    }
    i++;
  }
  return root;
}
*/


int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  int n;
  vector<int> tree;
  while (ss >> n) {
    tree.emplace_back(n);
  }
  sort(tree.begin(), tree.end());
  int key; cin >> key;
  string ans = "S";
  int left = 0;
  int right = tree.size() - 1;
  while (left <= right) {
    int mid = ceil((double) (left + right) / 2);
    if (tree[mid] == key) {
      ans += "Y";
      break;
    }
    if (left == right) {
      ans += "N";
      break;
    }
    if (tree[mid] < key) {
      left = mid + 1;
      ans += "R";
    } else {
      right = mid - 1;
      ans += "L";
    }
  }
  cout << ans << endl;
}