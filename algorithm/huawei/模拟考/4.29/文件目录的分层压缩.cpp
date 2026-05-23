/*
题目：文件目录的分层压缩

题目内容
现在有一个 n 层目录的文件系统，每个目录可以有多个文件和至多两个子目录（可视为二叉树）。
需要统计每个目录（包含其所有子目录）的总大小。

目录大小统计规则：
若某目录自身大小为 10，两个子目录大小分别为 3 和 5，则该目录统计后大小为 18。

输入描述
输入包含两行：

第一行：目录层数 n，取值范围 1 <= n <= 10。

第二行：基于二叉树层序遍历给出的第 0 ~ (n-1) 层各目录文件大小数组。
设数组元素为 m[i]，为整型，且 -1 <= m[i] <= 10。

其中：
- -1 表示该节点为空；
- 0 表示该节点无文件，但可能有子目录。

输出描述
输出基于二叉树层序遍历统计后的每个目录总大小。
若节点为空则输出 -1。
第 n-1 层最后一个节点后的空节点应省略，不输出。
输出结果末尾不能有多余空格。

样例1
输入
3
1 -1 2 -1 -1 3

输出
6 -1 5 -1 -1 3

说明
节点 3 为叶子节点，统计后为 3。
节点 2 只有一个子节点 3，统计后为 2 + 3 = 5。
根节点 1 有右子树统计值 5，统计后为 1 + 5 = 6。

样例2
输入
3
1 2 -1 5 3

输出
11 10 -1 5 3

说明
节点 5 和节点 3 无子节点，统计后分别为 5、3。
节点 2 有两个子节点 5 和 3，统计后为 2 + 5 + 3 = 10。
根节点 1 有左子树统计值 10，统计后为 1 + 10 = 11。
*/
#include <bits/stdc++.h>
#include <vector>
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left {nullptr};
  TreeNode *right {nullptr};

  TreeNode(int v) : val(v) {}
};

TreeNode *BuildTree(vector<int> &tree) {
  if (tree.empty() || tree[0] == -1) return nullptr;
  int i = 1;
  queue<TreeNode *> q;
  auto root = new TreeNode(tree[0]);
  q.push(root);

  while (!q.empty() && i < (int)tree.size()) {
    auto curr = q.front();
    q.pop();

    // 处理左孩子
    if (i < (int)tree.size()) {
      if (tree[i] != -1) {
        auto node = new TreeNode(tree[i]);
        if (curr) curr->left = node;
        q.push(node);
      } else {
        q.push(nullptr);   // 空节点也入队占位
      }
      i++;
    }
    // 处理右孩子
    if (i < (int)tree.size()) {
      if (tree[i] != -1) {
        auto node = new TreeNode(tree[i]);
        if (curr) curr->right = node;
        q.push(node);
      } else {
        q.push(nullptr);
      }
      i++;
    }
  }
  return root;
}

int main() {
  int n;
  cin >> n;
  vector<int> tree;
  cin.ignore();
  string input;
  getline(cin, input);
  stringstream ss(input);
  int tmp;
  while (ss >> tmp) {
    tree.emplace_back(tmp);
  }
  auto root = BuildTree(tree);
  auto subsum = [&](auto &self, TreeNode *curr) -> int {
    if (curr == nullptr) return 0;
    int sum = curr->val;
    sum += self(self, curr->left) + self(self, curr->right);
    curr->val = sum;
    return sum; 
  };
  subsum(subsum, root);
  queue<TreeNode *> q;
q.push(root);
vector<int> res;
int level = 0;
while (!q.empty() && level < n) {
  int size = q.size();
  for (int i = 0; i < size; ++i) {
    auto curr = q.front();
    q.pop();
    if (curr) {
      res.push_back(curr->val);
      q.push(curr->left);   // 不管左/右是不是 nullptr 都 push
      q.push(curr->right);
    } else {
      res.push_back(-1);
      q.push(nullptr);      // 空节点也要繁殖空孩子,保持位置对齐
      q.push(nullptr);
    }
  }
  level++;
}
  

  for (int i = res.size() - 1; i >= 0 && res[i] == -1; --i) {
    res.pop_back();
  }
  for (int i = 0; i < res.size(); ++i) {
    cout << res[i];
    if (i < res.size() - 1) cout << " ";
  }
}