#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 999999937;

vector<ll> fact, inv_fact;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

void precompute(int n) {
    fact.resize(n + 1);
    inv_fact.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++)
        fact[i] = fact[i - 1] * i % MOD;
    inv_fact[n] = power(fact[n], MOD - 2, MOD);
    for (int i = n - 1; i >= 0; i--)
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
}

ll C_small(ll n, ll k) {
    if (k < 0 || k > n || n < 0) return 0;
    if (n >= MOD) return 0; // should not happen for small calls
    return fact[n] % MOD * inv_fact[k] % MOD * inv_fact[n - k] % MOD;
}

ll C_lucas(ll n, ll k) {
    if (k < 0 || k > n) return 0;
    ll result = 1;
    while (n > 0 || k > 0) {
        ll ni = n % MOD;
        ll ki = k % MOD;
        if (ki > ni) return 0;
        result = result * C_small(ni, ki) % MOD;
        n /= MOD;
        k /= MOD;
    }
    return result;
}

int main() {
    ll n = 10000000000000LL;  // 10^13
    ll m = 1000000000000LL;   // 10^12
    ll d = 10000LL;           // 10^4

    precompute(MOD - 1);

    // C(n, m, d) = |C(n, d) * sum_{k=0}^{m-d-1} C(n-d, k) * (-1)^k| mod p
    ll binom_nd = C_lucas(n, d);

    // Using identity: sum_{k=0}^{M} (-1)^k C(N,k) = (-1)^M * C(N-1, M)
    ll N = n - d;      // 10^13 - 10^4
    ll M = m - d - 1;  // 10^12 - 10^4 - 1

    // C(N-1, M) mod p using Lucas
    ll binom_alt = C_lucas(N - 1, M);

    // C(n,m,d) = |C(n,d) * (-1)^M * C(N-1,M)|
    // Since we take absolute value, the sign doesn't matter:
    // C(n,m,d) = C(n,d) * C(N-1,M)
    ll answer = binom_nd * binom_alt % MOD;

    cout << answer << endl;

    return 0;
}
