/*
题目描述：
给定两个升序排列的整数链表，请将它们合并为一个新的升序链表并输出。
新链表中的节点应当由原来两个链表的节点拼接而成，并保持整体升序。
你只需要根据输入数据自行构造两个链表，然后输出合并后的链表中所有节点的值。

输入格式：
- 第一行包含一个整数 n（第一个链表的长度，可能为 0）。
- 第二行包含 n 个升序排列的整数，表示第一个链表各节点的值；当 n = 0 时，这一行可以为空行或直接省略读取。
- 第三行包含一个整数 m（第二个链表的长度，可能为 0）。
- 第四行包含 m 个升序排列的整数，表示第二个链表各节点的值；当 m = 0 时，这一行可以为空行或直接省略读取。

输出格式：
- 输出一行，为合并后的升序链表中所有节点的值，按从头到尾顺序输出，相邻两个数之间用一个空格分隔。
- 如果合并后的链表为空（即两个输入链表都为空），则输出为空行（直接输出换行即可）。

样例输入 1：
3
1 2 4
3
1 3 4

样例输出 1：
1 1 2 3 4 4

样例输入 2：
0

3
2 5 7

样例输出 2：
2 5 7

数据范围：
- 0 <= n, m <= 10^5
- 链表节点值为 32 位有符号整数范围内的任意整数
- 输入的两行链表数据各自保证为非降序（升序或有相等）
- 保证总输入规模在合理范围内，可在线性时间内完成合并
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

    ListNode* ConvergeSeqList(ListNode* headA, ListNode* headB) {
        ListNode* dummy = new ListNode;
        ListNode* head = dummy;
        while(headA && headB) {
            if(headA->val > headB->val) {
                dummy->next = headB;
                headB = headB->next;
            } else {
                dummy->next = headA;
                headA = headA->next;
            }
            dummy = dummy->next;
        }
        dummy->next = headA? headA : headB;
        return head->next;
    }
};

int main() {
    int m, n;
    cin >> m;
    vector<int> list(m);
    for(int i = 0; i < m; ++i) {
        cin >> list[i];
    }
    Solution::ListNode* l1 = nullptr;
    for(int i = m - 1; i >= 0; --i) {
        Solution::ListNode* node = new Solution::ListNode(list[i]);
        node->next = l1;
        l1 = node;
    }
    cin >> n;
    vector<int> list2(n);
    for(int i = 0; i < m; ++i) {
        cin >> list2[i];
    }
    Solution::ListNode* l2 = nullptr;
    for(int i = m - 1; i >= 0; --i) {
        Solution::ListNode* node = new Solution::ListNode(list2[i]);
        node->next = l2;
        l2 = node;
    }
    Solution sol;
    Solution::ListNode* res = sol.ConvergeSeqList(l1, l2);
    while(res) {
        cout << res->val << " ";
        res = res->next;
    }
}