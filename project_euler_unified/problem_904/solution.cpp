#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 904: Integer Partition Asymptotics
 *
 * Compute p(200) mod 10^9 + 7, where p(n) = number of integer partitions of n.
 *
 * Two methods:
 *   1. Standard DP (coin-change):   O(n^2) time, O(n) space
 *   2. Pentagonal number recurrence: O(n^{3/2}) time, O(n) space
 *
 * Identity: p(n) = sum_{j!=0} (-1)^{j+1} p(n - j(3j-1)/2)
 */

const long long MOD = 1e9 + 7;
const int N = 200;

/*
 * Method 1: Coin-change DP
 *
 * Each integer k in {1,...,n} acts as a "coin" with unlimited supply.
 * dp[j] accumulates the number of partitions of j using parts <= k.
 */
long long solve_dp() {
    vector<long long> dp(N + 1, 0);
    dp[0] = 1;
    for (int k = 1; k <= N; k++) {
        for (int j = k; j <= N; j++) {
            dp[j] = (dp[j] + dp[j - k]) % MOD;
        }
    }
    return dp[N];
}

/*
 * Method 2: Pentagonal number recurrence
 *
 * p(n) = p(n-1) + p(n-2) - p(n-5) - p(n-7) + p(n-12) + ...
 *
 * Generalized pentagonal numbers: w(j) = j(3j-1)/2 for j = 1,-1,2,-2,...
 * Sign pattern: +, +, -, -, +, +, ...
 */
long long solve_pentagonal() {
    vector<long long> p(N + 1, 0);
    p[0] = 1;

    for (int n = 1; n <= N; n++) {
        long long val = 0;
        int sign = 1;
        for (int j = 1; ; j++) {
            // Two pentagonal numbers per j
            int w1 = j * (3 * j - 1) / 2;  // positive j
            int w2 = j * (3 * j + 1) / 2;  // negative j

            if (w1 > n) break;
            val = (val + sign * p[n - w1] % MOD + MOD) % MOD;

            if (w2 <= n)
                val = (val + sign * p[n - w2] % MOD + MOD) % MOD;

            sign = -sign;
        }
        p[n] = val;
    }
    return p[N];
}

int main() {
    long long ans1 = solve_dp();
    long long ans2 = solve_pentagonal();

    // Cross-check both methods
    assert(ans1 == ans2);

    // Verify small known values
    // p(0)=1, p(1)=1, p(2)=2, p(3)=3, p(4)=5, p(5)=7
    vector<long long> dp(11, 0);
    dp[0] = 1;
    for (int k = 1; k <= 10; k++)
        for (int j = k; j <= 10; j++)
            dp[j] += dp[j - k];
    assert(dp[0] == 1 && dp[1] == 1 && dp[2] == 2);
    assert(dp[3] == 3 && dp[4] == 5 && dp[5] == 7);

    cout << ans1 << endl;
    return 0;
}
