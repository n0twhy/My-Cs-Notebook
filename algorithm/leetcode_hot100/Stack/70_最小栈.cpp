/*
 * LeetCode 155 - 最小栈 (Min Stack)
 * ACM 模式题面 - 仅题目与数据格式，请自行实现全部代码
 * =============================================================================
 *
 * 【题目描述】
 * 设计一个支持 push、pop、top 操作，并能在常数时间内检索到最小元素的栈。
 *
 * 实现 MinStack 类：
 * MinStack()      初始化堆栈对象。
 * void push(int val)   将元素 val 推入堆栈。
 * void pop()           删除堆栈顶部的元素。
 * int top()            获取堆栈顶部的元素。
 * int getMin()         获取堆栈中的最小元素。
 *
 * 【输入格式】
 * 输入包含多行：
 * 第一行是一个整数 n，表示操作次数。
 * 接下来的 n 行，每行包含一个操作：
 * - "push x"（x 是整数）：将 x 推入栈
 * - "pop"：弹出栈顶元素
 * - "top"：获取栈顶元素
 * - "getMin"：获取栈中最小元素
 *
 * 【输出格式】
 * 对于每个 "top" 和 "getMin" 操作，输出对应结果（每行一个整数）。
 * 其他操作无需输出。
 *
 * 【样例输入 1】
 * 7
 * push -2
 * push 0
 * push -3
 * getMin
 * pop
 * top
 * getMin
 *
 * 【样例输出 1】
 * -3
 * 0
 * -2
 *
 * （说明）
 * MinStack minStack = new MinStack();
 * minStack.push(-2);   // 栈：[-2]
 * minStack.push(0);    // 栈：[-2, 0]
 * minStack.push(-3);   // 栈：[-2, 0, -3]
 * minStack.getMin();   // 返回 -3
 * minStack.pop();      // 栈：[-2, 0]
 * minStack.top();      // 返回 0
 * minStack.getMin();   // 返回 -2
 *
 * 【数据范围】
 * -2^31 <= val <= 2^31 - 1
 * pop、top 和 getMin 操作总是在非空栈上调用
 * push、pop、top 和 getMin 最多被调用 3 * 10^4 次
 *
 * =============================================================================
 */

 #include <bits/stdc++.h>
 using namespace std;

class MinStack {
public:
  MinStack() {}
  int top() {
    return st.top();
  }
  void push(int val) {
    if (min_st.empty()) {
      st.push(val);
      min_st.push(val);
      return;
    }

    int curr_min = min_st.top();
    if (val <= curr_min) {
      min_st.push(val);
    }
    st.push(val);
  }
  void pop() {
    int curr = st.top();
    if (curr <= min_st.top()) {
      min_st.pop();
    }
    st.pop();
  }
  int getMin() {
    return min_st.top();
  }
private:
  stack<int> st;
  stack<int> min_st;
};

int main() {
  MinStack minStack;
  int n;
  cin >> n;
  cin.ignore();
  for (int i = 0; i < n; ++i) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    bool has_num = false;
    string opt;
    ss >> opt;
    if (opt == "push") {
      int n;
      ss >> n;
      minStack.push(n);
    }
    if (opt == "getMin") {
      cout << minStack.getMin() << endl;
    }
    if (opt == "pop") {
      minStack.pop();
    }
    if (opt == "top") {
      cout << minStack.top() << endl;
    }
  }
}