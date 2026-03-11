/*
 * 力扣 2 - 两数相加（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给你两个非空链表，表示两个非负整数。每位数字按逆序存储（个位在头节点），
 * 每个节点存一位数字。将这两个数相加，并以相同形式的链表返回。
 * 例：2→4→3 表示 342，5→6→4 表示 465，和为 807，结果链表为 7→0→8。
 *
 * 【输入格式】
 * - 第一行：两个整数 n、m，表示两个链表的节点个数。
 * - 第二行：n 个空格分隔的整数，为第一个链表从表头到表尾的每位数字（0~9）。
 * - 第三行：m 个空格分隔的整数，为第二个链表从表头到表尾的每位数字（0~9）。
 *
 * 【输出格式】
 * - 一行：结果链表从表头到表尾的数字，空格分隔。
 *
 * 【样例输入】
 * 3 3
 * 2 4 3
 * 5 6 4
 *
 * 【样例输出】
 * 7 0 8
 *
 * 【数据范围】
 * 1 <= n, m <= 100，节点数字 0~9，两数均无前导零（单独 0 允许）。
 */

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

    ListNode* TwoSum(ListNode* head_1, ListNode* head_2) {
        int carry = 0;
        ListNode* tail = new ListNode();
        ListNode* head = tail;
        while(head_1 || head_2 || carry) {
            int x = head_1 ? head_1->val : 0;
            int y = head_2 ? head_2->val : 0;
            int total = x + y + carry;
            int sum = total % 10;
            carry = total / 10;
            ListNode* node = new ListNode(sum);
            tail->next = node;
            tail = tail->next;
            if (head_1) head_1 = head_1->next;
            if (head_2) head_2 = head_2->next;
        }
        return head->next;

    }
    
};

int main() {
    int n, n_2;
    cin >> n >> n_2;
    Solution::ListNode* head_1 = new Solution::ListNode();
    Solution::ListNode* tail_1 = head_1;
    Solution::ListNode* head_2 = new Solution::ListNode();
    Solution::ListNode* tail_2 = head_2;
    for(int i = 0; i < n; ++i) {
        int v;
        cin >> v;
        Solution::ListNode* node = new Solution::ListNode(v);
        tail_1->next =  node;
        tail_1 = tail_1->next;
    }
    for(int i = 0; i < n_2; ++i) {
        int v;
        cin >> v;
        Solution::ListNode* node = new Solution::ListNode(v);
        tail_2->next =  node;
        tail_2 = tail_2->next;
    }
    Solution sol;
    Solution::ListNode* res = sol.TwoSum(head_1->next, head_2->next);
    while(res) {
        cout << res->val <<  " ";
        res = res->next;
    }

}