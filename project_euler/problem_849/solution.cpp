#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll MOD = 1e9 + 7;

ll power(ll base, ll exp, ll mod) {
    ll result = 1; base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod; exp >>= 1;
    }
    return result;
}

ll modinv(ll a, ll mod = MOD) { return power(a, mod - 2, mod); }

// Coupon collector: E[T] = n * H_n mod p
ll coupon_collector_mod(int n) {
    ll hn = 0;
    for (int k = 1; k <= n; k++)
        hn = (hn + modinv(k)) % MOD;
    return (ll)n % MOD * hn % MOD;
}

// Set cover via bitmask DP
int min_set_cover(int n, const vector<int>& masks) {
    int full = (1 << n) - 1;
    vector<int> dp(full + 1, n + 1);
    dp[0] = 0;
    for (int s = 0; s <= full; s++) {
        if (dp[s] > n) continue;
        for (int mask : masks) {
            int ns = s | mask;
            dp[ns] = min(dp[ns], dp[s] + 1);
        }
    }
    return dp[full];
}

int main() {
    // Verify E[T] for n=2 is 3
    // H_2 = 1 + 1/2 = 3/2, so 2 * 3/2 = 3
    ll h2 = (1 + modinv(2)) % MOD;
    assert(2 * h2 % MOD == 3);

    // Set cover: 3 elements, 3 sets
    vector<int> masks = {0b011, 0b110, 0b101};
    assert(min_set_cover(3, masks) == 2);

    cout << coupon_collector_mod(1000) << endl;
    return 0;
}
