/*
 * 力扣 234 - 回文链表（Palindrome Linked List）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给你一个单链表的头节点 head，请你判断该链表是否为回文链表。若是返回 true，否则返回 false。
 *
 * 【输入格式】
 * - 第一行：一个整数 n，表示链表节点个数。
 * - 第二行：n 个空格分隔的整数，为链表从表头到表尾的值。
 *
 * 【输出格式】
 * - 一行：是回文输出 1，否则输出 0。
 *
 * 【样例输入】
 * 4
 * 1 2 2 1
 *
 * 【样例输出】
 * 1
 *
 * 【样例输入 2】
 * 2
 * 1 2
 *
 * 【样例输出 2】
 * 0
 *
 * 【数据范围】
 * 1 <= n <= 10^5
 * 0 <= 节点值 <= 9
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
        if(!head) {
            return false;
        }
        if(!head->next) {
            return true;
        }
        ListNode* curr = head;
        ListNode* prev = nullptr;
        ListNode* next;
        while(curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }

    bool problems(ListNode* head) {
        ListNode* dummy = new ListNode;
        dummy->next = head;
        ListNode* slow = dummy;
        ListNode* fast = dummy;
        while(fast && fast->next && fast->next->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        ListNode* latter = reverseList(slow->next);
        slow->next = nullptr;
        ListNode* compare = head;
        while(latter && head) {
            if(latter->val != head->val) {
                return false;
            }
            latter = latter->next;
            head = head->next;
        } 
        return true;
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
    if(sol.problems(head)) {
        cout << 1 << endl;
    } else {
        cout << 0 << endl;
    }
}