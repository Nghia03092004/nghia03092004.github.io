#include <bits/stdc++.h>
using namespace std;

int main() {
    /*
     * Problem 171: Last 9 digits of sum of all n in (0, 10^20) with
     * f(n) = sum of squared digits being a perfect square.
     *
     * Digit DP: track (count, value_sum) keyed by digit-square-sum.
     */
    const long long MOD = 1000000000LL;
    const int D = 20;
    const int SMAX = D * 81; // 1620

    vector<long long> cnt(SMAX + 1, 0), tot(SMAX + 1, 0);
    cnt[0] = 1;

    for (int k = 0; k < D; k++) {
        vector<long long> nc(SMAX + 1, 0), nt(SMAX + 1, 0);
        for (int s = 0; s <= SMAX; s++) {
            if (cnt[s] == 0) continue;
            for (int d = 0; d <= 9; d++) {
                int ns = s + d * d;
                if (ns > SMAX) break;
                nc[ns] = (nc[ns] + cnt[s]) % MOD;
                nt[ns] = (nt[ns] + 10 * tot[s] % MOD + (long long)d * (cnt[s] % MOD)) % MOD;
            }
        }
        cnt = nc;
        tot = nt;
    }

    long long ans = 0;
    for (int j = 1; j * j <= SMAX; j++)
        ans = (ans + tot[j * j]) % MOD;

    cout << ans << endl;
    return 0;
}
