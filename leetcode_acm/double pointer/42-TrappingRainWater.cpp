#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
    int RainVol(vector<int>& nums) {
        int left = 1;
        int right = nums.size() - 2;
        int sum = 0;
        int leftMax = nums[0];
        int rightMax = nums[nums.size() - 1];
        while(left <= right) {
            leftMax = max(nums[left], leftMax);
            rightMax = max(nums[right], rightMax);
            if(leftMax < rightMax) {
                sum += leftMax - nums[left];
                left++;
            }else {
                sum += rightMax - nums[right];
                right--;
            }
        }
        return sum;            
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for(int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    Solution sol;
    int ans = sol.RainVol(nums);
    cout << ans << endl;
}
