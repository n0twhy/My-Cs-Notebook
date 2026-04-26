#include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
  public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;
        
        int max_num = *max_element(nums.begin(), nums.end());
        int min_num = *min_element(nums.begin(), nums.end());
        
        if (min_num == max_num) return 0;
        
        int avg_gap = ceil((double)(max_num - min_num) / (n - 1));
        vector<int> buckets_max(n, INT_MIN);
        vector<int> buckets_min(n, INT_MAX);
        for (auto &i : nums) {
            int seq = (i - min_num) / avg_gap;
            if (i < buckets_min[seq]) buckets_min[seq] = i;
            if (i > buckets_max[seq]) buckets_max[seq] = i;
        }

        int ans = INT_MIN;
        int prev_max = buckets_max[0];
        for (int i = 1; i < n; ++i) {
          if (buckets_min[i] == INT_MAX && buckets_max[i] == INT_MIN) continue;
          ans = max(buckets_min[i] - prev_max, ans);
          prev_max = buckets_max[i];
        }

        return ans;

      }
  };