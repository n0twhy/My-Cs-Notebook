/*
题目：新能源最高续航里程

题目内容
有从 1 到 n 按序编号的 n 辆纯电新能源汽车，给定一个总的电池容量 k。
请根据每辆新能源汽车的电池容量和续航里程情况，选取对应的新能源汽车组合，
满足所选组合内的新能源汽车电池容量总和不大于 k，且总续航里程最高。

输入描述
第一行输入整数 n，表示新能源汽车数量，范围为 1~50。
第二行输入整数 k，表示给定的总电池容量，范围为 1~1000。

接下来的输入给出每辆车的电池容量与续航里程信息：
- 第三行为 n 个整数，表示 1 到 n 号车的电池容量；
- 第四行为 n 个整数，表示 1 到 n 号车的续航里程。

其中：
- 电池容量范围为 1~100；
- 续航里程范围为 1~1000。

输出描述
按照编号从小到大输出所选组合中的新能源汽车编号，编号间以空格分隔。

注意：
1、如果没有满足要求的组合，输出 -1。
2、如果存在多个满足条件的组合均达到最高里程，则取总电量最少的组合输出。
3、在上述前提下，若存在多个组合均满足最少总电量，则取汽车数量最少的组合输出。
4、在上述前提下，题目可保证仅有一组组合。

样例1
输入
1
80
100
300

输出
-1

说明
该用例中不存在电量不大于 80 的组合，因此返回 -1。

样例2
输入
5
80
30 45 15 15 80
400 470 200 200 870

输出
1 2

说明
一共 5 辆车，电池容量分别为 30、45、15、15、80，
续航里程分别为 400、470、200、200、870，总电量要求不大于 80。

总电量不大于 80 的车辆组合包括：
(1)、(2)、(3)、(4)、(5)、(1,2)、(1,3)、(1,4)、(1,3,4)、(2,3)、(2,4)、(2,3,4)、(3,4)。

对应总电量依次为：
30、45、15、15、80、75、45、45、60、60、60、75、30。

对应总续航里程依次为：
400、470、200、200、870、870、600、600、800、670、670、870、400。

可知最高续航里程为 870，对应组合为 (5)、(1,2)、(2,3,4)。
在续航里程相同的情况下，取电量最小的组合为 (1,2)、(2,3,4)；
在电量也相同的情况下，取汽车数量最少的组合为 (1,2)。
因此输出 1 2。

样例3
输入
4
80
30 45 50 60
400 470 450 600

输出
1 2

说明
一共 4 辆车，电池容量分别为 30、45、50、60，
续航里程分别为 400、470、450、600，总电量要求不大于 80。

总电量不大于 80 的组合可以是：
1（总电量 30）、1和2（总电量 75）、1和3（总电量 80）、
2（总电量 45）、3（总电量 50）、4（总电量 60）。

对应总续航里程分别为：
400、870、850、470、450、600。

因此最长续航里程为 870，对应组合为 1 和 2。
最终输出 1 2。
*/

#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> problem(int limit, vector<int> &energy, vector<int> &miles) {
    for (int i = 0; i < energy.size(); ++i) {
      cars_[i + 1] = {energy[i], miles[i]};
    }

    vector<pair<vector<int>, pair<int, int>>> res; 
    function<void(int, int, vector<int>&, int)> dfs = [&](int curr_energy, int curr_miles, vector<int> &curr_cars, int start) {
      for (int i = start; i < energy.size(); ++i) {
        if (curr_energy + cars_[i + 1].first < limit) {
          curr_cars.emplace_back(i + 1);
          dfs(curr_energy + cars_[i + 1].first, curr_miles + cars_[i + 1].second, curr_cars, i + 1);
          curr_cars.pop_back();
        } else {
          res.push_back({curr_cars, {curr_energy, curr_miles}});
        }
      }
    };
    vector<int> curr_cars;
    dfs(0, 0, curr_cars, 0);
    sort(res.begin(), res.end(), [](pair<vector<int>, pair<int, int>> &a, pair<vector<int>, pair<int, int>> &b){
      if (a.second.second != b.second.second) return a.second.second > b.second.second;
      if (a.second.first != b.second.first) return a.second.first < b.second.first;
      return a.first.size() < b.first.size();
    });


    return res[0].first;
  }

private:
  unordered_map<int, pair<int, int>> cars_;
};

int main() {
  int n, limit;
  cin >> n >> limit;
  vector<int> energy(n);
  vector<int> miles(n);

  for (int i = 0; i < n; ++i) {
    cin >> energy[i];
  }

  for (int i = 0; i < n; ++i) {
    cin >> miles[i];
  }

  Solution sol;
  auto res = sol.problem(limit, energy, miles);

  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << " ";
  }
}