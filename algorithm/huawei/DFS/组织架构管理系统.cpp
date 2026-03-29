/*
题目：组织架构管理系统

题目内容
你正在开发一个企业的组织架构管理系统。公司的组织架构被简化表示为一棵二叉树：树的每个节点代表一名员工；树的层级越深（离根越远）则员工级别越低，级别高者为主管，管辖其子树中的下属。

每名员工有唯一工号，为整数，范围 [0, 10^9]。

系统需提供统计：对任意两名员工，求他们「级别最低」的共同主管（即二叉树上的最近公共祖先 LCA），并输出该主管所管辖的人员总个数。

注意：
- 若两名员工存在上下级关系，则共同主管为其中级别较高者（即祖先节点）。
- 员工总数 <= 10^5。
- 两名待查工号一定在树中，且互不相同。

输入描述
第一行：整数 n，表示节点个数（层序展开后的数组长度）。

第二行：n 个整数，空格分隔，表示一棵二叉树按层序、从左到右的各节点工号；-1 表示该位置为空节点。
（原题「满一叉树」按语义应为二叉树的层序存储。）

例：1 2 3 4 -1 -1 5 -1 -1 -1 6 对应一棵按层编号的二叉树（-1 为空位）。

第三行：两个整数，空格分隔，为待查询的两名员工工号。

输出描述
输出该共同主管所管辖的人员总个数，不含主管自身（即其子孙节点个数，或等价地：以其为根的子树大小减 1）。

样例1
输入：
12
2 3 5 1 7 6 8 -1 -1 0 4 -1
0 3

输出：
4

说明：0 与 3 存在上下级关系，共同主管为 3；3 的子孙共 4 人，故输出 4。

样例2
输入：
9
1 2 3 4 -1 5 -1 -1 6
5 4

输出：
5

说明：5 与 4 的最近公共祖先为 1；以 1 为根的子树除自身外共 5 人，故输出 5。
*/

#include <bits/stdc++.h>
#include <charconv>
#include <cstdlib>
using namespace std;

class Solution {
public:
  struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int v): val(v), left(nullptr), right(nullptr) {};
  };

  TreeNode *FindAncestor(TreeNode *root, int a, int b) {
    if (root->val == a || root->val == b) {
      return root;
    }
    TreeNode *tmp_left = nullptr;
    TreeNode *tmp_right = nullptr;
    if(root->left) tmp_left = FindAncestor(root->left, a, b);
    if(root->right) tmp_right = FindAncestor(root->right, a, b);
    TreeNode *res = nullptr;
    if (tmp_left || tmp_right) {
      if (tmp_left && tmp_right) return root;
      res = tmp_left == nullptr ? tmp_right : tmp_left;
      return res;
    } else {
      return nullptr; 
    }
  }

  int ChildrenCount(TreeNode *root) {
    int size = 1;
    if (root->left) size += ChildrenCount(root->left);
    if (root->right) size += ChildrenCount(root->right);
    return size;
  }

  int SuperVisor(TreeNode *root, int a, int b) {
    auto supervisor = FindAncestor(root, a, b);
    int res = ChildrenCount(supervisor);
    return res - 1;
  }
};


int main() {
  int n;
  cin >> n;
  vector<int> staff(n);
  for (int i = 0; i < n; ++i) {
    cin >> staff[i];
  }
  auto root = new Solution::TreeNode(staff[0]);
  queue<pair<Solution::TreeNode *, int>> q;
  q.push({root, 0});
  while (!q.empty()) {
    auto [curr_node, pos] = q.front();
    q.pop();
    int left = 2 * pos + 1;
    int right = 2 * pos + 2;
    if (left < n && staff[left] != -1) {
      curr_node->left = new Solution::TreeNode(staff[left]);
      q.push({curr_node->left, left});
    }
    if (right < n && staff[right] != -1) {
      curr_node->right = new Solution::TreeNode(staff[right]);
      q.push({curr_node->right, right});
    }
  }
  int a, b;
  cin >> a >> b;
  Solution sol;
  int res = sol.SuperVisor(root, a, b);
  cout << res << endl;
}
