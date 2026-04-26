#include <bits/stdc++.h>
#include <cctype>
using namespace std;

bool isLeap(int y) {
  if ((y % 4 == 0) && (y % 100 != 0) || (y % 400) == 0) {
    return true;
  }
  return false;
}

int days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void nextday(int &y, int &m, int &d) {
  d++;
  int dim = (m == 2 && isLeap(y)) ? 29 : days[m];
  if (d > dim) {
    d = 1;
    m++;
    if (m > 12) {
      m = 1;
      y++;
    }
  }
}
//例 1:2000-1-1 到 2020-10-1 一共多少天?

int CountDays() {
  int y = 2000, m = 1, d = 1;
  int days = 0;
  while (!(y == 2020 && m == 10 && d == 1)) {
    nextday(y, m, d);
    days++;
  }
  cout << days << endl;
  return 0;
}

long long ToDec(string s, int k) {
  long long res = 0;
  for (auto &c : s) {
    int digit = isdigit(c) ? c - '0' : 10 + c - 'A';
    res = res * k + digit;
  }
  return res;
}

string FromDec(int num, int k) {
  string res;
  while (num) {
    int digit = num % k;
    res += digit > 10 ? ('A' + digit - 10) : '0' + digit;
    num /= k;
  }
  reverse(res.begin(), res.end());
  return res;
}

int main() {
  cout << ToDec("64", 16) << endl;
  cout << FromDec(1000, 16) << endl;
  cout << FromDec(ToDec("64", 16), 2);
}