#include <bits/stdc++.h>
#include <utility>
#include <vector>
using namespace std;

void QuickSort(vector<int>& nums, int l, int r) {
    if (l >= r) return;
    int i = l, j = r;
    int pivot = nums[(l + r) / 2];
    while (i <= j) {
        while (nums[i] < pivot) i++;
        while (nums[j] > pivot) j--;
        if (i <= j) {
            swap(nums[i], nums[j]);
            i++;
            j--;
        }
    }
    if (l < j) QuickSort(nums, l, j);
    if (i < r) QuickSort(nums, i, r);
}