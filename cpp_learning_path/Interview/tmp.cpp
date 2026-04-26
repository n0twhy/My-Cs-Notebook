#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int up = 1;
    int down = 1;
    for (int i = 1; i < a.size(); ++i) {
        if (a[i] > a[i - 1]) {
            up = down + 1;
        }
        else if (a[i] < a[i - 1]) {
            down = up + 1;
        }
    }
    int L = max(up, down);
    cout << n - L << endl;
    
}