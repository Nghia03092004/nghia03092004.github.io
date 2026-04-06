#include <bits/stdc++.h>
using namespace std;

// Problem 178: Step Numbers
// Count pandigital step numbers (containing all digits 0-9) with up to 40 digits.
// DP: dp[d][mask] = count of step numbers ending in digit d with digit set mask.

int main() {
    const int MAXN = 40;
    const int FULL = (1 << 10) - 1;  // 1023

    // dp[d][mask]
    long long dp[10][1024] = {};
    long long ndp[10][1024] = {};

    // Base case: 1-digit numbers (d = 1..9, no leading zero)
    for (int d = 1; d <= 9; d++) {
        dp[d][1 << d] = 1;
    }

    long long answer = 0;

    // Check if 1-digit numbers are pandigital (they can't be)
    for (int d = 0; d <= 9; d++) {
        answer += dp[d][FULL];
    }

    // Extend from length n to n+1
    for (int n = 1; n < MAXN; n++) {
        memset(ndp, 0, sizeof(ndp));
        for (int d = 0; d <= 9; d++) {
            for (int mask = 0; mask <= FULL; mask++) {
                if (dp[d][mask] == 0) continue;
                long long val = dp[d][mask];
                if (d >= 1) {
                    ndp[d-1][mask | (1 << (d-1))] += val;
                }
                if (d <= 8) {
                    ndp[d+1][mask | (1 << (d+1))] += val;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));

        // Count pandigital step numbers of length n+1
        for (int d = 0; d <= 9; d++) {
            answer += dp[d][FULL];
        }
    }

    cout << answer << endl;
    return 0;
}
