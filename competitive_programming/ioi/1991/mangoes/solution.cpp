// IOI 1991 - Problem 2: Mangoes
// Weighted job scheduling: each tree has interval [d, d+k-1] with value a.
// Pick at most one tree per day to maximize total harvest.
// DP with binary search, sorted by deadline. O(n log n)
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    scanf("%d%d", &n, &k);

    struct Tree {
        int start, end, value;
    };
    vector<Tree> trees(n);
    for (int i = 0; i < n; i++) {
        int d, a;
        scanf("%d%d", &d, &a);
        trees[i] = {d, d + k - 1, a};
    }

    // Sort by end time (deadline)
    sort(trees.begin(), trees.end(),
         [](const Tree& a, const Tree& b) { return a.end < b.end; });

    // dp[i] = max mangoes considering first i trees
    vector<long long> dp(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        dp[i] = dp[i - 1]; // skip tree i-1

        // Binary search: find last tree j whose end < tree i's start
        int lo = 0, hi = i - 1, best = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (trees[mid].end < trees[i - 1].start) {
                best = mid + 1;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        dp[i] = max(dp[i], dp[best] + trees[i - 1].value);
    }

    printf("%lld\n", dp[n]);
    return 0;
}
