/*
题目内容
你来到食堂想大吃一顿。你将在全荤、半荤、素中各挑选一种菜品，且所挑选菜品的食材不能重复，请你计算菜品选择方式。

输入描述
食堂的所有餐线的菜单：

第 1 行是：
N，其中 N 为所有菜品数量，范围 (0, 100]。

接下来 N 行中，每行格式为：
IDi Ti DI1 DI2 DI3 DI4

其中：
- IDi：第 i 个菜品的菜品名，范围 (0, 100]。
- Ti：菜品类型，1 表示全荤、2 表示半荤、3 表示素。
- DI1...DI4：第 i 个菜品的第 1 到第 4 种食材，食材不会重复，范围 [0, 100]，
  其中 0 表示占位（无食材）。

输出描述
输出菜品选择的所有方式列表。排序规则：
1. 先按全荤菜品名升序
2. 再按半荤菜品名升序
3. 再按素菜品名升序

每行格式为：
全荤菜品名 半荤菜品名 素菜品名

如果无法选择，输出：
-1

样例1
输入：
1
1 1 1 2 3 4

输出：
-1

说明：
仅有 1 个菜，无法按要求挑选。

样例2
输入：
6
2 2 5 6 7 8
3 3 9 10 11 12
4 2 1 2 3 0
5 1 5 0 7 8
1 1 1 2 3 4
6 1 1 2 3 4

输出：
1 2 3
5 4 3
6 2 3

说明：
总共有 6 种菜品。

菜品名 2：半荤，食材 5、6、7、8
菜品名 3：素，食材 9、10、11、12
菜品名 4：半荤，食材 1、2、3
菜品名 5：全荤，食材 5、7、8
菜品名 1：全荤，食材 1、2、3、4
菜品名 6：全荤，食材 1、2、3、4

菜品 1、菜品 4、菜品 6 食材有相同部分，不能同时选择。
菜品 2 和菜品 5 食材有相同部分，不能同时选择。

可选择方式有：
5+4+3，1+2+3，6+2+3
共 3 种。

按升序排序后输出：
1 2 3
5 4 3
6 2 3

样例3
输入：
4
1 1 1 2 3 4
2 2 5 6 7 8
3 3 9 10 11 12
4 2 13 14 15 16

输出：
1 2 3
1 4 3

说明：
总共有 4 种菜品。

菜品名 1：全荤，食材 1、2、3、4
菜品名 2：半荤，食材 5、6、7、8
菜品名 3：素，食材 9、10、11、12
菜品名 4：半荤，食材 13、14、15、16

由于食材都不重复，可选择方式有：
1+2+3，1+4+3
共 2 种。

提示
由于中餐的博大精深，同样的食材选择可做出不同的菜品。
但同一个菜品名一定是同样的食材。
*/

#include <bits/stdc++.h>
#include <bitset>
#include <string>
#include <utility>
using namespace std;

class Solution {
public:

  struct diet {
    bitset<101> mask;
    int type;
  };
  vector<vector<int>> dietSelection(vector<vector<int>>& diets) {
    if (diets.size() <= 1) return {{-1}};

    vector<bitset<101>> masks;
    vector<pair<int, diet>> meat;
    vector<pair<int, diet>> half;
    vector<pair<int, diet>> vege;
    
    for (auto& v : diets) {
      bitset<101> mask;
      for (int i = 2; i < v.size(); ++i) {
        if(v[i] != 0) mask.set(v[i]);
      }
      diet curr {mask, v[1]};
      if (curr.type == 1) meat.emplace_back(v[0],curr);
      if (curr.type == 2) half.emplace_back(v[0], curr);
      if (curr.type == 3) vege.emplace_back(v[0], curr);
    }

    if (meat.size() == 0 || half.size() == 0 || vege.size() == 0) return {{-1}};

    vector<vector<int>> res;
    for (int i = 0; i < meat.size(); ++i) {
      for (int j = 0; j < half.size(); ++j) {
        for (int k = 0; k < vege.size(); ++k) {
          if (!(meat[i].second.mask & half[j].second.mask).any() && !(meat[i].second.mask & vege[k].second.mask).any() && !(half[j].second.mask & vege[k].second.mask).any()) {
            res.push_back({meat[i].first, half[j].first, vege[k].first});
          }
        }
      }
    }

    sort(res.begin(), res.end(), [](const vector<int>& a, const vector<int>& b){
      if (a[0] != b[0]) return a[0] < b[0];
      if (a[1] != b[1]) return a[1] < b[1];
      return a[2] < b[2];
    });

    return res;
  }
};

int main() {
  int n;
  cin >> n;
  cin.ignore();
  vector<vector<int>> diets(n);
  for (int i = 0; i < n; ++i) {
    string tmp;
    getline(cin, tmp);
    stringstream ss(tmp);
    int tmp_i;
    while (ss >> tmp_i) {
      diets[i].emplace_back(tmp_i);
    }
    
  }

  Solution sol;
  vector<vector<int>> res = sol.dietSelection(diets);
  if (res[0][0] == -1) return -1;
  for (int i = 0; i < res.size(); ++i) {
    for (int j = 0; j < res[i].size(); ++j) {
      cout << res[i][j] << " ";
    }
    cout << endl;
  }
}