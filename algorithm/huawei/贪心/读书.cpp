/*
题目描述

小明是个爱学习的孩子，并且喜好读书，并且更喜欢读好书，在某一天他要读 n 本书，读每一本书都要花费 1 单位时间，
并且小明对每一本书有一个满意值，同时读完每一本书小明都会收获一定的快乐值，
现在小明想知道要按照怎样的顺序读书、读哪些书，能使他收获的快乐值总和最大。

每本书籍的快乐值为：读这本书和之前所有书所花费的时间乘以对这本书的满意值，可按照任意顺序读书，也可以放弃读某些书。

设书的总数为 n，书的满意值为 m：
1<=n<=500
-1000<=m<=1000

输入描述
输入为字符串，字符串内容为代表每本书籍的满意值，
例如输入为 "4,3,2"，表示有 3 本书，满意值分别是 4、3 和 2。

输出描述
输出为数字，代表最大的快乐值之和。

样例

输入1
4,3,2
输出1
20
说明
按照原来顺序相反的时间阅读书籍（2*1 + 3*2 + 4*3 = 20），满意值越高的书籍越后面读可以获得最大的快乐值。

输入2
-1,-4,-5
输出2
0
说明
这些书籍满意值都很低，所以不阅读任何书籍可以获得最大的快乐值。

输入3
-1,-8,0,5,-9
输出3
14
说明
去掉第二个和最后一本书籍，最大的快乐时间系数和为（-1*1 + 0*2 + 5*3 = 14），每本书籍都需要花费 1 单位时间来阅读。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

int main() {
  string input;
  getline(cin, input);
  stringstream ss(input);
  string token;
  vector<int> books;
  
  while (getline(ss, token, ',')) {
    books.emplace_back(stoi(token));
  }

  sort(books.begin(), books.end());

  /*for (int i = 0; i < books.size(); ++i) {
    cout << books[i] << " ";
  }
  cout << "\n";*/

  int add_sum = 0;
  int ans = 0;
  for (int i = books.size() - 1; i >= 0; --i) {
    add_sum += books[i];

    if (add_sum > 0) {
      ans += add_sum;
    } else {
      break;
    }
  }
  cout << ans << endl;
  
}
