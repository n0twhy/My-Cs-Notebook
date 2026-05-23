/*
题目描述
你正在为一个社交网络平台开发好友推荐功能。

平台上有 N 个用户（每个用户使用 1 到 N 的整数编号），系统中维护了用户之间的好友关系。
为了推荐新朋友，平台决定采用“共同好友数量”作为衡量两个用户之间相似度的标准。

系统根据输入用户编号 K，输出与此用户 K 相似度最高的前 L 个用户 ID，来推荐给用户 K。

相似度定义：两个用户为“非好友”时，这两个用户的相似度为他们拥有的共同好友数。
例如用户 A 和用户 B 只有共同好友 C 和 D，则相似度 = 2。

输入描述
第一行包含四个整数 N、M、K 和 L，分别表示：
- N：用户的数量；
- M：好友记录条数；
- K：查询的用户编号；
- L：需要推荐的好友数量。

接下来有 M 行，每行包含两个整数 X 和 Y，表示编号为 X 和 Y 的用户是好友。

说明：
1. 输入格式都是标准的，无需考虑异常场景（不会包含用户和自己是好友的输入，例如 1 1）。
2. 用户数不超过 1024，用户编码最大不超过 10244。
3. 好友记录数不超过 10240。

输出描述
根据输入的 K 和 L，输出和用户 K 相似度最高的 L 个用户编号。

要求：
1. 输出相似度最高的前 L 个用户编号，按照“相似度从高到低”排序；
2. 如果有相似度相同的可能好友，则按用户编号从小到大排序；
3. 如果推荐的好友个数不足 L 个，则需要推荐与用户 K 无共同好友关系的用户（陌生人）作为可能好友；
   若推荐仍不足 L 个用户，则剩余推荐用户编号使用 0 来占位补足。

样例1
输入：
6 7 3 2
1 2
1 3
2 3
3 4
3 5
4 5
5 6

输出：
6 0

解释：
共有 6 个用户，7 条好友记录，给用户编号为 3 的用户推荐 2 个好友。
只有编号为 6 的用户可能是编号 3 用户的可能好友；
尝试推荐与编号 3 用户无共同好友的其他用户时，
除编号为 6 的用户外，其他用户和编号 3 用户都是好友，所以找不到陌生人作为第二个推荐用户；
推荐结果不足 2 个用户，所以推荐的第二个用户编码使用 0 占位补足。

样例2
输入：
8 11 1 3
1 2
1 3
2 3
3 4
3 5
4 5
5 6
6 7
7 8
1 8
2 7

输出：
7 4 5

解释：
共有 8 个用户，11 条好友记录，给用户编号为 1 的用户推荐 3 个好友。
按照相似度排序推荐给用户 1 的相关好友为：7、4、5。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <functional>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int main() {
   int n, m, k, l;
   cin >> n >> m >> k >> l;

   vector<bitset<1025>> users(n + 1, 0);
   while (m--) {
      int user1, user2;
      cin >> user1 >> user2;
      users[user1].set(user2);
      users[user2].set(user1);      
   }
   auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) -> bool {
      if (a.first != b.first) return a.first > b.first;
      return a.second < b.second;
   };
   priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);
   for (int i = 1; i < users.size(); ++i) {
      if (i == k || users[k].test(i)) continue;
      auto co_friend = users[i] & users[k];
      int cnt = co_friend.count();
      if (pq.size() < l) {
         pq.push({cnt, i});
         continue;
      }

      if (cnt == pq.top().first && i < pq.top().second) {
         pq.pop();
         pq.push({cnt, i});
      }

      if (cnt > pq.top().first) {
         pq.pop();
         pq.push({cnt, i});
      }
   }
   vector<pair<int, int>> ans;
   while (!pq.empty()) {
      auto [cnt, user] = pq.top();
      pq.pop();
      ans.emplace_back(cnt, user);
   }
   sort(ans.begin(), ans.end(), [](const pair<int, int> &a, const pair<int, int> &b){
      if (a.first != b.first) return a.first > b.first;
      return a.second < b.second;
   });

   for (int i = 0; i < l; ++i) {
      if (i < ans.size()) {
         cout << ans[i].second << " ";
      } else {
         cout << 0 << " ";
      }
   }
}
