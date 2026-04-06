#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Simple battlement: no two adjacent columns same height, heights in [1, h]
ll simple_battlement(int w, int h) {
    if (w == 0) return 0;
    return (ll)h % MOD * power(h - 1, w - 1, MOD) % MOD;
}

// Full castle DP: battlement + at least one column reaches height h
// State: (last_height, reached_top)
ll full_castle(int w, int h) {
    if (w == 0) return 0;

    // dp[j][t]: count ending at height j, t=reached_top
    vector<vector<ll>> dp(h + 1, vector<ll>(2, 0));

    // Base: first column
    for (int j = 1; j <= h; j++) {
        int t = (j == h) ? 1 : 0;
        dp[j][t] = 1;
    }

    for (int col = 2; col <= w; col++) {
        // Compute totals for each t
        ll total[2] = {0, 0};
        for (int j = 1; j <= h; j++) {
            for (int t = 0; t < 2; t++) {
                total[t] = (total[t] + dp[j][t]) % MOD;
            }
        }

        vector<vector<ll>> new_dp(h + 1, vector<ll>(2, 0));
        for (int j2 = 1; j2 <= h; j2++) {
            for (int t = 0; t < 2; t++) {
                int t_new = (t == 1 || j2 == h) ? 1 : 0;
                ll contrib = (total[t] - dp[j2][t] % MOD + MOD) % MOD;
                new_dp[j2][t_new] = (new_dp[j2][t_new] + contrib) % MOD;
            }
        }
        dp = new_dp;
    }

    ll result = 0;
    for (int j = 1; j <= h; j++) {
        result = (result + dp[j][1]) % MOD;
    }
    return result;
}

int main() {
    // Test cases
    printf("Simple battlement C(w, h):\n");
    for (int w = 1; w <= 13; w++) {
        printf("  C(%d, 10) = %lld\n", w, simple_battlement(w, 10));
    }

    printf("\nFull castle (must reach top):\n");
    for (int w = 1; w <= 13; w++) {
        printf("  C(%d, 10) = %lld\n", w, full_castle(w, 10));
    }

    printf("\nC(13, 10) = %lld\n", full_castle(13, 10));

    return 0;
}
