/*
题目内容

玩家小A现在有一个 n×m 个格子的背包（n 为高，m 为宽），有 t 个物品，先按排序算法把物品进行排序，然后根据排序后的物品一一放入背包。

在放入背包时，优先选择行数最小的位置进行放置，其次选择列数最小的位置。如果当前行无法容纳物品，则继续查找下一行。如果整个背包都无法放下该物品，则跳过该物品。

最后输出背包是否放的下所有物品。并输出当前背包状态，用 id 表示当前背包格子是什么物品，如果当前格子没有物品输出 0。

每个物品都包含以下这些属性：
1. id：物品唯一 ID，每个物品各不相同。
2. height*width：物品占用的格子数。（height 为高，width 为宽）物品不可以旋转放入背包。
3. quality：物品的品质。数值越大品质越高。
4. type：物品的种类。

排序规则如下：
1. 优先按照 type 排序，"equip" > "weapon" > "item" > "other"。
2. 品质高的优先。
3. id 小的优先。

输入描述

第一行包含三个整数 n (1≤n≤50)，m (1≤m≤50)，t (1≤t≤1000)，分别表示背包的高，背包的宽，物品的数量。

接下来 t 行，每行包含四个整数，id (1≤id≤10000)，height (1≤height≤5)，width (1≤width≤5)，quality (1≤quality≤7)，一个字符串 type (type∈{"equip","weapon","item","other"})，为物品包含的属性。

输出描述

第一行包含一个字符串 "YES" 表示为背包可以放下所有物品。"NO" 表示为有的物品放不进去背包。

接下来 n 行，每行包含 m 个整数，表示当前格子放着的物品 id，如果当前格子没有物品则输出 0。

样例1
输入
4 3 3
1 1 3 7 other
2 3 1 2 equip
3 2 2 4 other

输出
YES
2 3 3
2 3 3
2 0 0
1 1 1

说明
排序后顺序：2 1 3。按顺序放入即可。

样例2
输入
4 4 6
6 2 1 3 weapon
1 2 3 1 equip
3 2 1 4 item
5 3 3 3 weapon
2 2 2 3 other
4 2 1 3 item

输出
NO
1 1 1 6
1 1 1 6
3 4 2 2
3 4 2 2
*/

#include <bits/stdc++.h>
#include <functional>
#include <utility>
#include <vector>
using namespace std;


enum ObjectType {
  other = 1,
  item = 2,
  weapon = 3,
  equip = 4
};

struct Object {
  int id;
  int height;
  int width;
  int quality;
  ObjectType type;
};

ObjectType ParseType(const string &s) {
  if (s == "other") return other;
  if (s == "item") return item;
  if (s == "weapon") return weapon;
  return equip;
}


pair<int, int> CanPut(vector<vector<int>> &bag, int height, int width) {
  int n = 0;
  bool is_put = true;
  for (int row = 0; row <= bag.size() - height; ++row) {
    for (int col = 0; col <= bag[row].size() - width; ++col) {
      for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
          if (bag[row + i][col + j] != 0) is_put = false;
        }
      }
      if (is_put) return {row, col};
      is_put = true;
    }
  }
  return {-1, -1};
}

void Put(vector<vector<int>> &bag, int row, int col, int id, int height, int width) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      bag[row + i][col + j] = id;
    }
  }
}

int main() {
  int n, m, item_cnt;
  cin >> n >> m >> item_cnt;
  vector<Object> objects(item_cnt);
  for (int i = 0; i < item_cnt; ++i) {
    int id, height, width, quality;
    string type;
    cin >> id >> height >> width >> quality >> type;
    Object ob{id, height, width, quality, ParseType(type)};
    objects[i] = ob;
  }
  sort(objects.begin(), objects.end(), [](Object &a, Object &b) {
    if (a.type != b.type) return a.type > b.type;
    if (a.quality != b.quality) return a.quality > b.quality;
    return a.id < b.id;
  });

  vector<Object> put_ins;
  
  vector<vector<int>> bag(n, vector<int>(m, 0));

  for (int i = 0; i < objects.size(); ++i) {
    auto p = CanPut(bag, objects[i].height, objects[i].width);
    if (p.first != -1) {
      put_ins.emplace_back(objects[i]);
      Put(bag, p.first, p.second, objects[i].id, objects[i].height, objects[i].width);
    }
  }

  if (put_ins.size() == objects.size()) {
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }

  for (int i = 0; i < bag.size(); ++i) {
    for (int j = 0; j < bag[0].size(); ++j) {
      cout << bag[i][j] << " ";
    }
    cout << "\n";
  }
}