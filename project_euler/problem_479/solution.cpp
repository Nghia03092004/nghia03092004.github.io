#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1000000007LL;

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

ll modinv(ll a, ll mod) {
    return power(a, mod - 2, mod);
}

int main() {
    ll n = 1000000; // 10^6

    ll ans = 0;

    // For k=1: 1 - k^2 = 0, so contribution is 0
    // For k >= 2:
    // sum_{p=1}^{n} (1-k^2)^p = ((1-k^2)^{n+1} - (1-k^2)) / ((1-k^2) - 1)
    //                          = ((1-k^2)^{n+1} - (1-k^2)) / (-k^2)

    for (ll k = 2; k <= n; k++) {
        ll k2 = k % MOD * (k % MOD) % MOD;
        ll q = (1 - k2 % MOD + MOD) % MOD; // q = 1 - k^2 mod MOD

        // Numerator: q^{n+1} - q
        ll qn1 = power(q, n + 1, MOD);
        ll num = (qn1 - q + MOD) % MOD;

        // Denominator: q - 1 = -k^2
        // So we divide by (q - 1) = -k^2
        // num / (q - 1) = num / (-k^2) = -num * inv(k^2)
        ll inv_k2 = modinv(k2, MOD);
        ll term = (MOD - num) % MOD * inv_k2 % MOD;

        ans = (ans + term) % MOD;
    }

    printf("%lld\n", ans);

    return 0;
}
