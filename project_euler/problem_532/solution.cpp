#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 532: Nanoscale Strips
 *
 * f(W,H) = C(W+H, W)^2 mod p,  p = 999999937.
 *
 * Precompute factorials and inverse factorials mod p, then evaluate.
 * Time: O(W+H).  Space: O(W+H).
 */

const ll MOD = 999999937LL;
const int MAXN = 20000001;

ll fact[MAXN], inv_fact[MAXN];

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

void precompute(int n) {
    fact[0] = 1;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;
    inv_fact[n] = power(fact[n], MOD - 2, MOD);
    for (int i = n - 1; i >= 0; i--)
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
}

ll binom(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] % MOD * inv_fact[k] % MOD * inv_fact[n - k] % MOD;
}

int main() {
    int W = 10000000, H = 10000000;
    precompute(W + H);
    ll c = binom(W + H, W);
    ll ans = c * c % MOD;
    cout << ans << endl;
    return 0;
}
