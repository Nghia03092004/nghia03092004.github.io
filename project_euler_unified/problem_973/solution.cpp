#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 100000;
    vector<int> nums;
    for (int i = 1; i < N; i += 2) nums.push_back(i);
    int idx = 1;
    while (idx < (int)nums.size() && nums[idx] <= (int)nums.size()) {
        int step = nums[idx];
        vector<int> next;
        for (int i = 0; i < (int)nums.size(); i++)
            if ((i+1) % step != 0) next.push_back(nums[i]);
        nums = next;
        idx++;
    }
    long long ans = 0;
    for (int x : nums) ans += x;
    cout << ans << endl;
    return 0;
}
