/*
题目内容
在一条数轴上有 n（1 < n < 1000）个人，第 i 个人的出生点为 p_i，终点为 q_i（1 ≤ p_i < q_i < 10^9）。所有人的出生点两两不同，且按从小到大的顺序输入（p_1 < p_2 < ... < p_n）；对于任意 i，都有 p_i < q_i，即所有人都只会沿数轴正方向移动。

最开始，第 1 个人持有妖伞，并从自己的出生点 p_1 出发前往终点 q_1。

移动过程中，所有已加入的人会形成一个队伍，并按照以下规则行动：

加入队伍：当当前持伞队伍经过某个人的出生点 p_i 时，如果此人尚未加入过队伍，则该人立即加入到队伍末尾（「经过」包括恰好停在该点的情况）。

离开队伍：当某个人到达自己的终点 q_i 时，该人会立刻离开队伍，并且之后不会再次加入队伍。

妖伞交接：如果离开队伍的人恰好是当前持有妖伞的人，若此时队伍非空，则妖伞交给新的队首；若此时队伍为空，则妖伞会停留在当前位置不动，此后所有尚未加入过队伍的人中，出生点距离妖伞当前位置最近的人会先前往该位置取得妖伞，再继续前往自己的终点。若该人在前往取伞途中经过了其他尚未加入过队伍的人的出生点，这些人同样会按规则加入队伍末尾。

最近人的唯一性：题目保证所有人的出生点互不相同且严格递增，因此当队伍为空时，距离妖伞最近且尚未加入的人是唯一确定的。

定义第 i 个人的贡献值为该人持有妖伞期间，妖伞实际发生位移的总距离。请输出每个人的贡献值。

输入描述
第一行输入一个整数 n（1 < n < 1000），表示人数。

接下来 n 行，每行输入两个整数 p_i、q_i（1 ≤ p_i < q_i < 10^9），表示第 i 个人的出生点和终点，满足 p_1 < p_2 < ... < p_n。

输出描述
输出一行 n 个整数，第 i 个整数表示第 i 个人持有妖伞期间，妖伞实际发生位移的总距离。

样例1
输入
4
1 10
5 6
9 30
20 25
输出
9 0 20 0
说明
初始时，第 1 个人在位置 1 持有妖伞并出发。当队伍移动到位置 5 时，第 2 个人加入；当队伍移动到位置 9 时，第 3 个人加入；第 1 个人到达终点 10 后离开，贡献为 10 - 1 = 9。

随后第 2 个人成为新的队首，但其终点 6 已在当前位置左侧，立刻离队，贡献为 0。队伍继续前进，第 4 个人在位置 20 加入；第 4 个人到达终点 25 时离队，在此之前未成为持伞者，贡献为 0。最终第 3 个人持有妖伞从位置 10 移动到终点 30，贡献为 30 - 10 = 20。
*/

#include <bits/stdc++.h>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
  struct Tuple {
    int start;
    int end;
    int pos;
  };
  vector<int> Transfer(vector<pair<int, int>> &inputs) {
    vector<Tuple> people;
    for (int i = 0; i < inputs.size(); ++i) {
      people.emplace_back(Tuple{inputs[i].first, inputs[i].second, i});
    }
    sort(people.begin(), people.end(), [](Tuple &a, Tuple &b) {
      return a.start < b.start;
    });

    int um_contribution = 0;
    queue<Tuple> q;
    q.push(people[0]);
    int seq = 1;
    int last_end = people[0].start;
    vector<int> res(inputs.size());
    while (!q.empty()) {
      int start = last_end;
      int end = q.front().end;
      int pos = q.front().pos;
      q.pop();

      while (seq < people.size() && end >= people[seq].end) {
        res[people[seq].pos] = 0;
        seq++;
      }

      res[pos] = end - start;
      last_end = end;
      
      if (seq < people.size()) {
        q.push(people[seq++]);
      }
    }
    return res;
  }
};

int main() {
  int n;
  cin >> n;
  vector<pair<int, int>> inputs(n);
  for (int i = 0; i < n; ++i) {
    cin >> inputs[i].first >> inputs[i].second;
  }
  Solution sol;

  vector<int> res = sol.Transfer(inputs);
  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
}