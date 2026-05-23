#include <bits/stdc++.h>
#include <memory>
#include <vector>
using namespace std;

int main() {
    int *p = new int(1);
    char *x = (char*)&p;
    if (*x == 1) {
        cout << "小端" << endl;
    } else {
        cout << "大端" << endl;
    }
}


