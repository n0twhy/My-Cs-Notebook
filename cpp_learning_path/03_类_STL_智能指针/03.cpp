#include <iostream>
#include <vector>
int main() {
    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    vec.push_back(4);

    std::cout << *it << std::endl;
}