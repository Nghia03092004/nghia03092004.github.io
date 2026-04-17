#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

ll mod_inv(ll a, ll mod) {
    return mod_pow(a, mod - 2, mod);
}

// Find primitive root of prime p
ll primitive_root(ll p) {
    ll phi = p - 1;
    // Factor phi
    vector<ll> factors;
    ll n = phi;
    for (ll d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            factors.push_back(d);
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) factors.push_back(n);

    for (ll g = 2; g < p; g++) {
        bool ok = true;
        for (ll f : factors) {
            if (mod_pow(g, phi / f, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

// S(n, k, m) mod p using roots of unity filter
// Requires k | (p-1)
ll S_mod(ll n, ll k, ll m, ll p) {
    assert((p - 1) % k == 0);

    ll g = primitive_root(p);
    ll omega = mod_pow(g, (p - 1) / k, p);
    ll inv_k = mod_inv(k, p);

    ll total = 0;
    for (ll j = 0; j < k; j++) {
        ll gj;
        if (j == 0) {
            gj = m % p;
        } else {
            ll w = mod_pow(omega, j, p);
            ll wm = mod_pow(w, m, p);
            // gj = w * (1 - w^m) / (1 - w)
            ll num = w % p * ((1 - wm % p + p) % p) % p;
            ll den = (1 - w % p + p) % p;
            gj = num % p * mod_inv(den, p) % p;
        }
        total = (total + mod_pow(gj, n, p)) % p;
    }

    return total % p * inv_k % p;
}

int main() {
    // Simple case: S(n, k) = k^(n-1) when m = k
    cout << "Simple case verification:" << endl;
    for (int n = 1; n <= 5; n++) {
        for (int k : {2, 3, 5}) {
            ll result = 1;
            for (int i = 0; i < n - 1; i++) result *= k;
            cout << "  S(" << n << ", " << k << ") = " << result << endl;
        }
    }

    // General case with modular arithmetic
    ll p = 1000000007;  // 10^9 + 7
    ll k = 7;
    // Check k | (p-1): (10^9 + 6) % 7 = ?
    if ((p - 1) % k == 0) {
        ll n = 100, m = 20;
        ll result = S_mod(n, k, m, p);
        cout << "\nS(" << n << ", " << k << ", " << m << ") mod " << p
             << " = " << result << endl;
    }

    return 0;
}
