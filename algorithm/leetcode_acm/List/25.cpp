/*
 * LeetCode 25 - K 个一组翻转链表 (Reverse Nodes in k-Group)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 给你链表的头节点 head，每 k 个节点一组进行翻转，返回修改后的链表。
 * 若最后不足 k 个节点，则保持原有顺序不变。
 *
 * 【输入格式】
 * 第一行：一个整数 n，表示链表节点个数。
 * 第二行：n 个空格分隔的整数，表示链表从左到右的节点值。
 * 第三行：一个整数 k，表示每组翻转的节点个数。
 *
 * 【输出格式】
 * 一行，翻转后的链表节点值从左到右，相邻数字之间用一个空格分隔，行末可有空格。
 *
 * 【样例输入 1】
 * 5
 * 1 2 3 4 5
 * 2
 *
 * 【样例输出 1】
 * 2 1 4 3 5
 *
 * 【样例输入 2】
 * 5
 * 1 2 3 4 5
 * 3
 *
 * 【样例输出 2】
 * 3 2 1 4 5
 *
 * 【数据范围】
 * 链表节点数 [1, 5000]
 * 0 <= 节点值 <= 1000
 * 1 <= k <= n
 *
 * =============================================================================
 */
#include <vector>
#include <iostream>
using namespace std;

class Solution {
public:
struct ListNode {
  int val;
  ListNode* next;

  ListNode() : val(0), next(nullptr) {}
  ListNode(int v) : val(v), next(nullptr) {}
};
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* curr = head;
        for (int i = 0; i < k; ++i) {
            if(!curr) return head;
            curr = curr->next;
        }
        ListNode* node = head;
        ListNode* prev = nullptr;
        for (int i = 0; i < k; ++i) {
            ListNode* nextTmp = node->next;
            node->next = prev;
            prev = node;
            node = nextTmp;
        }

        head->next = reverseKGroup(curr, k);
        return prev;

    }
};

int main() {
  int n;
  cin >> n;
  vector<int> list(n);
  for (int i = 0; i < n; ++i) {
    cin >> list[i];
  }
  Solution::ListNode* head = new Solution::ListNode (list[n - 1]);
  for (int i = n - 2; i >= 0; --i) {
    Solution::ListNode* node = new Solution::ListNode(list[i]);
    node->next = head;
    head = node;
  }
  int k;
  cin >> k;

  Solution sol;
  Solution::ListNode* res = sol.reverseKGroup(head, k);
  while (res) {
    cout << res->val << " ";
    res = res->next;
  }
}