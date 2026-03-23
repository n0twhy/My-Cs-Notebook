/*
 * 力扣 206 - 反转链表（Reverse Linked List）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给你单链表的头节点 head，请你反转链表，并返回反转后的头节点。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示链表节点个数。
 * - 第二行：n 个空格分隔的整数，为链表从表头到表尾的值。
 *
 * 【输出格式】
 * - 一行：n 个空格分隔的整数，为反转后链表从表头到表尾的值。
 *
 * 【样例输入】
 * 5
 * 1 2 3 4 5
 *
 * 【样例输出】
 * 5 4 3 2 1
 *
 * 【数据范围】
 * 0 <= n <= 5000
 * -5000 <= 节点值 <= 5000
 */

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
    ListNode* reverseList(ListNode* head) {
        ListNode* curr = head;
        ListNode* next = curr;
        ListNode* prev = nullptr;
        while(curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> list(n);
    for(int i = 0; i < n; ++i) {
        cin >> list[i];
    }
    Solution::ListNode* head = nullptr;
    for(int i = n - 1; i >= 0; --i) {
        Solution::ListNode* node = new Solution::ListNode(list[i]);
        node->next = head;
        head = node;
    }
    Solution sol;
    Solution::ListNode* res = sol.reverseList(head);
    while(res) {
        cout << res->val << " ";
        res = res->next;
    }
}