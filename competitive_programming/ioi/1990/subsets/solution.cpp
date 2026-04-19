// IOI 1990 - Problem 1: Subsets
// Find all subsets of n integers that sum to target T.
// Uses meet-in-the-middle for efficiency (handles n up to ~40).
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, target;
    scanf("%d%d", &n, &target);
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int half = n / 2;
    int other = n - half;

    // Generate all subset sums for the first half
    map<int, vector<vector<int>>> leftSums;
    for (int mask = 0; mask < (1 << half); mask++) {
        int s = 0;
        vector<int> subset;
        for (int i = 0; i < half; i++) {
            if (mask & (1 << i)) {
                s += arr[i];
                subset.push_back(arr[i]);
            }
        }
        leftSums[s].push_back(subset);
    }

    // Generate all subset sums for the second half, look up complement
    for (int mask = 0; mask < (1 << other); mask++) {
        int s = 0;
        vector<int> subset;
        for (int i = 0; i < other; i++) {
            if (mask & (1 << i)) {
                s += arr[half + i];
                subset.push_back(arr[half + i]);
            }
        }
        int need = target - s;
        auto it = leftSums.find(need);
        if (it != leftSums.end()) {
            for (auto& left : it->second) {
                if (left.empty() && subset.empty()) continue; // skip empty subset
                printf("{ ");
                for (int x : left) printf("%d ", x);
                for (int x : subset) printf("%d ", x);
                printf("}\n");
            }
        }
    }
    return 0;
}
