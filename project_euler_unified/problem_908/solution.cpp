#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 908: Coprime Chains
 *
 * Find the longest coprime chain in {2, 3, ..., 100}.
 * A coprime chain has gcd(a_i, a_{i+1}) = 1 for consecutive elements.
 *
 * Key theorem: gcd(n, n+1) = 1 for all n, so the entire sequence
 * 2, 3, ..., 100 forms a valid chain of length 99.
 *
 * Two methods:
 *   1. Direct observation: answer = N - 1
 *   2. DP verification: dp[i] = max chain ending at i
 */

int main() {
    int N = 100;

    // Method 1: Direct
    int ans_direct = N - 1;

    // Method 2: DP verification
    vector<int> dp(N + 1, 0);
    for (int i = 2; i <= N; i++) {
        dp[i] = 1;
        for (int j = 2; j < i; j++) {
            if (__gcd(j, i) == 1) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }
    int ans_dp = *max_element(dp.begin() + 2, dp.end());

    assert(ans_direct == ans_dp);

    // Verify consecutive coprimality
    for (int k = 2; k < N; k++) {
        assert(__gcd(k, k + 1) == 1);
    }

    cout << ans_direct << endl;
    return 0;
}
