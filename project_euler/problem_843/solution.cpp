#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 843: Periodic Tilings
 *
 * Count domino tilings of m x n grid via profile DP.
 * Profile = bitmask of m bits showing which cells extend to next column.
 */

typedef long long ll;

int M, N;

void fill_column(int profile_in, int profile_out, int row,
                 vector<pair<int,int>>& transitions) {
    if (row == M) {
        transitions.push_back({profile_in, profile_out});
        return;
    }
    if ((profile_in >> row) & 1) {
        fill_column(profile_in, profile_out, row + 1, transitions);
    } else {
        // Horizontal domino
        fill_column(profile_in, profile_out | (1 << row), row + 1, transitions);
        // Vertical domino
        if (row + 1 < M && !((profile_in >> (row + 1)) & 1)) {
            fill_column(profile_in, profile_out, row + 2, transitions);
        }
    }
}

ll solve(int m, int n) {
    if ((ll)m * n % 2 == 1) return 0;
    M = m; N = n;
    if (M > N) swap(M, N);

    // Build all valid transitions
    vector<pair<int,int>> transitions;
    for (int mask = 0; mask < (1 << M); mask++) {
        fill_column(mask, 0, 0, transitions);
    }

    // DP
    vector<ll> dp(1 << M, 0);
    dp[0] = 1;
    for (int col = 0; col < N; col++) {
        vector<ll> ndp(1 << M, 0);
        for (auto [from, to] : transitions) {
            ndp[to] += dp[from];
        }
        dp = ndp;
    }
    return dp[0];
}

int main() {
    // Verify known values
    assert(solve(2, 3) == 3);
    assert(solve(4, 4) == 36);
    assert(solve(2, 10) == 89);
    assert(solve(8, 8) == 12988816);

    cout << solve(8, 8) << endl;
    return 0;
}
