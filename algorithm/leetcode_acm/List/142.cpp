/*
 * 力扣 142 - 环形链表 II（Linked List Cycle II）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给定一个链表的头节点 head，如果链表有环，返回环的入口节点；如果无环，返回 null（本题用 -1 表示）。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示链表节点个数。
 * - 第二行：n 个空格分隔的整数，为链表从表头到表尾的值。
 * - 第三行：一个整数 pos，表示尾节点 next 指向的节点下标（0 为第一个节点）；若 pos = -1，表示无环。
 *
 * 【输出格式】
 * - 一行：环的入口节点的值；若无环输出 -1。
 *
 * 【样例输入】
 * 4
 * 3 2 0 -4
 * 1
 *
 * 【样例输出】
 * 2
 *
 * 【样例说明】
 * 环的入口为下标 1 的节点，值为 2。
 *
 * 【数据范围】
 * 0 <= n <= 10^4
 * -10^5 <= 节点值 <= 10^5
 * pos = -1 或 0 <= pos < n
 */

 #include <unordered_set>
 #include <iostream>
 #include <vector>
 using namespace std;
 class Solution {
 public:
     struct ListNode {
      int val;
      ListNode* next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int v) : val(v), next(nullptr) {}
     };
     ListNode* linkedListCycle(ListNode* head) {
         unordered_set<ListNode*> mp;
         ListNode* tail = head;
         while(tail) {
             if(mp.count(tail)) {
                 return tail;
             }else {
                 mp.insert(tail);
             }
             tail = tail->next;
             
         }  
         return nullptr;
         
     }
 };
 
int main() {
  int n;
  cin >> n;
  vector<int> list(n);
  for(int i = 0; i < n; ++i) {
      cin >> list[i];
  }
  Solution::ListNode* tail = new Solution::ListNode(list[n-1]);
  Solution::ListNode* head = tail;
  for(int i = n - 2; i >= 0; --i) {
      Solution::ListNode* node = new Solution::ListNode(list[i]);
      node->next = head;
      head = node;
  }
  int pos;
  cin >> pos;
  Solution::ListNode* curr = head;
  if(pos != -1) {
      while(pos) {
          curr = curr->next;
          pos--;
      }
      tail->next = curr;
  }
  Solution sol;
  Solution::ListNode* res = sol.linkedListCycle(head);
  if(res) {
      cout << res->val << endl;
  }else {
      cout << -1 << endl;
  }
}