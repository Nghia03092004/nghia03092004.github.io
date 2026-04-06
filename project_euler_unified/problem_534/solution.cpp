#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 534: Weak Queens
 *
 * Place n non-attacking weak queens (king-move attacks only) on n x n board.
 * One queen per row. Queens in adjacent rows must differ by >= 2 columns.
 *
 * Bitmask DP: dp[mask][last_col] = count of ways.
 * Time: O(n^2 * 2^n).
 */

ll f(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    int total_masks = 1 << n;
    // dp[mask][last_col]
    vector<vector<ll>> dp(total_masks, vector<ll>(n, 0));

    // Row 0: place queen in any column
    for (int c = 0; c < n; c++) {
        dp[1 << c][c] = 1;
    }

    for (int row = 1; row < n; row++) {
        vector<vector<ll>> new_dp(total_masks, vector<ll>(n, 0));
        for (int mask = 0; mask < total_masks; mask++) {
            if (__builtin_popcount(mask) != row) continue;
            for (int prev_c = 0; prev_c < n; prev_c++) {
                if (dp[mask][prev_c] == 0) continue;
                for (int c = 0; c < n; c++) {
                    if (mask & (1 << c)) continue;
                    if (abs(c - prev_c) < 2) continue;
                    new_dp[mask | (1 << c)][c] += dp[mask][prev_c];
                }
            }
        }
        for (int mask = 0; mask < total_masks; mask++) {
            for (int c = 0; c < n; c++) {
                dp[mask][c] += new_dp[mask][c];
            }
        }
    }

    int full = (1 << n) - 1;
    ll result = 0;
    for (int c = 0; c < n; c++) {
        result += dp[full][c];
    }
    return result;
}

int main() {
    ll total = 0;
    for (int n = 1; n <= 12; n++) {
        ll val = f(n);
        printf("f(%d) = %lld\n", n, val);
        total += val;
    }
    printf("Sum = %lld\n", total);
    assert(total == 3884);
    cout << total << endl;
    return 0;
}
