#include <iostream>
#include <vector>
using namespace std;
int main() {
    int* p = nullptr;
    cout << sizeof(p) << endl;
    char* c = nullptr;
    cout << sizeof(c) << endl;
    vector<int> nums = {0,1,2,3,4,5,6,7,8,9};
    int* v = &nums[0];
    v++;
    cout << sizeof(v)  << " and address: " << v << endl;
    *v = 2;
    for(int i = 0; i < nums.size(); ++i) {
        cout << nums[i] << " ";
    }
}