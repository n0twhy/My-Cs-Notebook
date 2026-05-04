/*
题目描述

小明将对 10^5 个通道下发任务。

当用户输入 -1 时，分配没有任务且编号最小的通道给用户。
当用户输入有效的通道编号时，回收并清空此通道。

请你计算出若干次操作后，下一次用户申请时将得到的通道编号，
若无可以使用的通道，返回 -1。

输入格式
第一行为一个整数 n，表示用户输入次数。

接下来一行 n 个整数，分别表示每一次操作用户的输入数，
通道编号在 0 到 50000 内。

1<=n<=10^5

输出格式
输出下一次将申请到的通道编号，若无可使用的通道，输出 -1。

Sample Input 1
7
-1 -1 1 -1 0 -1 2

Sample Output 1
2
*/

#include <bits/stdc++.h>
#include <queue>
#include <vector>
using namespace std;

int main() {
  int n;
  cin >> n;

  int next_return = 0;
  priority_queue<int, vector<int>, greater<int>> pq;

  while (n--) {
    int op;
    cin >> op;
    if (op == -1) {
      if (pq.size() == 0) next_return++;
      else pq.pop();
    } else {
      pq.push(op);
    }
  }

  if (pq.size() == 0) {
    cout << next_return << endl;
  } else {
    cout << pq.top() << endl;
  }
}
