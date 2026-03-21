/*
 * 力扣 160 - 相交链表（Intersection of Two Linked Lists）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 给你两个单链表的头节点 headA 和 headB，请你找出并返回两个单链表相交的起始节点。
 * 如果两个链表没有交点，返回 null（本题用 -1 表示无交点）。
 * 题目保证链表无环。
 *
 * 【输入格式】
 * - 第一行：两个整数 n、m，表示链表 A 和链表 B 的节点个数。
 * - 第二行：n 个空格分隔的整数，为链表 A 从表头到表尾的值。
 * - 第三行：m 个空格分隔的整数，为链表 B 从表头到表尾的值。
 * - 第四行：两个整数 posA、posB（0 为第一个节点），表示相交节点在 A 中的下标、在 B 中的下标。
 *   若两链表不相交，则 posA = -1、posB = -1。
 *
 * 【输出格式】
 * - 一行：相交节点的值；若不相交则输出 -1。
 *
 * 【样例输入】
 * 5 6
 * 4 1 8 4 5
 * 5 6 1 8 4 5
 * 2 3
 *
 * 【样例输出】
 * 8
 *
 * 【样例说明】
 * A: 4->1->8->4->5，B: 5->6->1->8->4->5，在值为 8 的节点相交。
 *
 * 【数据范围】
 * 1 <= n, m <= 3 * 10^4
 * 1 <= 节点值 <= 10^5
 * posA in [0, n-1] 或 -1，posB in [0, m-1] 或 -1；若不为 -1，则 A[posA] 与 B[posB] 为同一节点（值相同且之后序列一致）。
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

    ListNode* Crossing(ListNode* headA, ListNode* headB) {
        int m = 0;
        int n = 0;
        ListNode* tmp = headA;
        while(tmp) {
            m++;
            tmp = tmp->next;
        }
        tmp = headB;
        while(tmp) {
            n++;
            tmp = tmp->next;
        }
        ListNode* longer = nullptr;
        ListNode* shorter = nullptr;
        int diff = 0;
        if (m > n) {
            longer = headA;
            shorter = headB;
            diff = m - n;
        } else {
            longer = headB;
            shorter = headA;
            diff = n - m;
        }
        while(diff) {
            longer = longer->next;
            diff--;
        }
        while(longer && shorter && longer != shorter) {
            longer = longer->next;
            shorter = shorter->next;
        }
        return longer;yij
    }
};

int main() {
    int n, m, posA, posB;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];
    cin >> posA >> posB;

    if (posA < 0 || posB < 0) {
        cout << -1 << endl;
        return 0;
    }

    // 先建公共尾：A[posA] -> A[posA+1] -> ... -> A[n-1]
    Solution::ListNode* tail = nullptr;
    for (int i = n - 1; i >= posA; i--) {
        Solution::ListNode* node = new Solution::ListNode(a[i]);
        node->next = tail;
        tail = node;
    }
    // 建链表 A：A[0] -> ... -> A[posA-1] -> tail
    Solution::ListNode* headA = tail;
    for (int i = posA - 1; i >= 0; i--) {
        Solution::ListNode* node = new Solution::ListNode(a[i]);
        node->next = headA;
        headA = node;
    }
    // 建链表 B：B[0] -> ... -> B[posB-1] -> tail（同一段）
    Solution::ListNode* headB = tail;
    for (int i = posB - 1; i >= 0; i--) {
        Solution::ListNode* node = new Solution::ListNode(b[i]);
        node->next = headB;
        headB = node;
    }

    Solution sol;
    Solution::ListNode* ans = sol.Crossing(headA, headB);
    cout << (ans ? ans->val : -1) << endl;
    return 0;
}