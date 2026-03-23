/*
题目：查找可用内存块

有一个由若干个连续的内存单元组成的一个环形的内存块，我们现在需要从这个内存块中某个内存单元后申请指定大小的连续内存。

为了方便描述我们进行以下约定：

我们用一个 10 进制 byte 数字序列来描述这个内存块的现状；其中每一个数字为 1 个 byte byte，表示内存块中 8 个连续的内存单元，它的每一个 bit bit 的值用于描述 1 个内存单元是否使用：
    - bit 为 0 时表示内存已使用
    - bit 为 1 表示空闲

我们对环形内存块的每一个内存单元进行编号，假设数字序列共有 n 个数字，数字序列的第 1 个数的 bit0 bit0 到 bit7 bit7，依次对应编号为 0~7 的内存单元；第 2 个数的 bit0 bit0 到 bit7 bit7 依次对应编号为 8~15 的内存单元，依次类推。第 n 个数字对应 8n-8 ~ 8n-1 的内存单元。

从 0 到 8n-1 编号的内存单元按照物理上连续，编号越大，内存单元越高。编号 0 和 8n-1 的两个首尾内存单元相邻构成环状的内存。

现在需要在这个连续的内存块中的指定编号为 m 的内存单元之后（顺时针），找到长度为 k 的连续未使用的内存块，按以下规则优先进行匹配：

查找方向是从 [m+1, 8n-1] 和 [0, m] 回环次序查找满足条件的连续未使用的内存单元段。

当有多个内存段满足条件时，优先在大小最接近申请大小连续内存段中，申请起始编号距离 m 最近的内存段。

编号距离计算方式如下：
    - j > m 时，距离 j - m
    - j < m 时，距离 j + 8n - m

输入描述
输入是一个字符串数字序列，至少包括 2 行：

    第 1 行有两个数字：
        - 第 1 个数字：申请的连续单元个数 k，范围：0 ~ 65535
        - 第 2 个数字：n，表示数字序列长度（内存块字节数），范围一般由题面限定（与后续 8n 的编号一致）

    第 2 行有两个数字：
        - m，在这个编号的内存单元后开始查找，范围：0 ~ 3600

从第 2 行开始为描述内存块的数字序列（可能存在换行符）。
数字之间可用空格或者换行隔开，最多 500 个数字，每个数字范围是 0 ~ 255。

例如输入：
    1
    2
    1 2
    128
    64

表示在 bit bit 序列（编号 0~15）为：
    10000000
    10000000
    10000000 10000000
（从左到右对应编号低到高）的内存块中，从编号为 2 的内存单元（第 2 个 byte bit）后申请 1 个内存单元。

输出描述
输出申请满足申请的内存段的起始内存单元编号。
如果不存在满足的内存段，输出 -1。

样例 1
输入
    3 6
    59 143
输出
    15
说明：
样例的目的是从状态为 11011100 11110001（从左到右依次为编号 0~15 的内存单元使用状态）内存块中
从编号为 6 的内存单元后申请 3 个连续的内存单元。

经扫描找出 3 个内存段满足这点，分别为：
    编号 8 到编号 11 有 4 个内存单元
    编号 15 到编号 1 有 3 个内存单元
    编号 3 到编号 5 有 3 个内存单元

其长度最匹配的段是第 2 段和第 3 段，在这里面距离编号为 6 的距离分别是：
    第 3 段：距离为 9
    第 2 段：距离为 3 + 16 - 6 = 13

最匹配的是第 2 段，在其内申请 3 的内存单元，起始地址为 15。

样例 2
输入
    3 1
    6 17
输出
    8
说明：
样例的目的是从状态为 10111100 11110000（从左到右依次为编号 0~15 的内存单元使用状态）内存块中
申请 3 个连续的内存单元，从编号为 1 的内存单元后开始进行申请。

经过查找有 2 个内存段满足这点，分别为：
    编号 2 到编号 5 有 4 个内存单元
    编号 8 到编号 10 有 3 个内存单元

其中长度最匹配的是第 2 段，在这里面距离编号为 1 的最近的内存段是 8~10，因此结果为 8。

样例 3
输入
    3 1
    0 0
输出
    -1
说明：
样例的目的是从状态为 00000000 00000000（从左到右依次为编号 0~15 的内存单元使用状态）内存块中
申请 3 个连续的内存单元，从编号为 1 的内存单元后开始进行申请。

经过查找没有一个连续的空内存块有 3 个连续未使用内存单元，因此结果为 -1。

样例 4
输入
    2 1
    0 254
输出
    9
说明：
样例的目的是从状态为 00000000 01111111（从左到右依次为编号 0~15 的内存单元（用 0 表示空）内存块中
申请 2 个连续的内存单元。从编号为 1 的内存单元后开始进行申请。

经过计算，只有 1 个连续的空内存块：从编号 9 到编号 15。可以申请 2 个连续未使用的内存单元。
这里面编号为 10 的内存单元最近的内存是 9~10。因此结果为 9。

提示：
    可能 m 的大小小于内存单元的个数，此时返回 1；
    在失败场景下，返回 -1。
*/

#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <utility>
using namespace std;

class Solution {
public:
  int memoryBlock(pair<int, int>& search, vector<int> memory) {
    int index = search.second;
    int units = search.first;
    int n = memory.size();
    string memory_binary = "";
    for (int& i : memory) {
      string tmp = bitset<8>(i).to_string();
      reverse(tmp.begin(), tmp.end());
      memory_binary += tmp;
    }
    
    vector<pair<int, int>> available;
    int total = memory_binary.size();
    int start = (index + 1) % total;

    for (int step = 0; step < total; ++step) {
      int i = (start + step) % total;
      if (memory_binary[i] != '1') continue;
      
      int len = 0;
      while (len < total && memory_binary[(i + len) % total] == '1') {
        len++;
      }
      step += len;
      if (len >= units) {
        available.emplace_back(i, len);
        
      }
      
    }
    
    if (available.size() == 0) {
      return -1;
    } 
    int best_start = -1;
    int best_diff = INT_MAX;  // len - units
    int best_dist = INT_MAX;  // 与 m 的环形距离

    for (const auto &p : available) {
        int start = p.first;
        int len = p.second;

        int diff = len - units;
        int dist = (start > index) ? (start - index) : (start + 8 * n - index);

        // 优先级：diff 最小 -> dist 最小 -> start 最小（稳定性）
        if (diff < best_diff ||
            (diff == best_diff && dist < best_dist) ||
            (diff == best_diff && dist == best_dist && start < best_start)) {
            best_diff = diff;
            best_dist = dist;
            best_start = start;
        }
    }

    return best_start;

  }
};

int main() {
  int units, index;
  cin >> units >> index;
  cin.ignore();

  vector<int> memory;
  string all;
  getline(cin, all);
  stringstream ss(all);
  int x;
  while (ss >> x) {
    memory.emplace_back(x);
  }

  Solution sol;
  pair<int, int> tmp_p = {units, index};
  int res = sol.memoryBlock(tmp_p, memory);
  cout << res << endl;
}