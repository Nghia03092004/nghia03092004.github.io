#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2020 - Biscuits
// DP on bits: process from bit 0 to bit K-1.
// State: carry of leftover biscuits propagated upward.
// At bit i, total available = a[i] + carry.
// For each bit value b in {0,1}: need b*x biscuits for bags,
// remainder carries up as floor((total - b*x) / 2).
// Count distinct tastiness values y.

ll count_tastiness(ll x, vector<ll> a) {
    // Extend to 61 bits to handle all possible values
    while ((int)a.size() < 61) a.push_back(0);
    int K = (int)a.size();

    // dp[carry] = number of distinct y-value suffixes achievable
    map<ll, ll> dp;
    dp[0] = 1;

    for (int i = 0; i < K; i++) {
        map<ll, ll> ndp;
        for (auto& [carry, ways] : dp) {
            ll total = a[i] + carry;
            // Bit i of y can be 0 or 1
            for (int b = 0; b <= 1; b++) {
                ll need = (ll)b * x;
                if (total < need) continue;
                ll remaining = total - need;
                ll new_carry = remaining / 2;
                ndp[new_carry] += ways;
            }
        }
        dp = ndp;
    }

    ll ans = 0;
    for (auto& [carry, ways] : dp)
        ans += ways;
    return ans;
}

int main() {
    int q;
    scanf("%d", &q);
    while (q--) {
        ll x;
        int k;
        scanf("%lld %d", &x, &k);
        vector<ll> a(k);
        for (int i = 0; i < k; i++) scanf("%lld", &a[i]);
        printf("%lld\n", count_tastiness(x, a));
    }
    return 0;
}
