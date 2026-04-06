/*
 * Problem 899: Tree Enumeration
 * Cayley's formula: t(n) = n^(n-2) labeled trees on n vertices.
 * Computes cumulative sum modulo a prime using fast exponentiation.
 */
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

// Cayley: n^(n-2) mod p
ll cayley_mod(ll n, ll mod) {
    if (n <= 1) return 0;
    return power(n, n - 2, mod);
}

// Brute force for small n (Kirchhoff via determinant)
ll kirchhoff_K_n(int n) {
    // For K_n, result is n^(n-2)
    ll result = 1;
    for (int i = 0; i < n - 2; i++) result *= n;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verification
    cout << "=== Cayley's Formula ===" << endl;
    for (int n = 2; n <= 12; n++) {
        ll cayley = 1;
        for (int i = 0; i < n - 2; i++) cayley *= n;
        cout << "t(" << n << ") = " << cayley << endl;
    }

    // Cumulative sum mod p
    cout << "\n=== Cumulative Sum mod 10^9+7 ===" << endl;
    for (int N : {10, 100, 1000, 10000, 100000}) {
        ll s = 0;
        for (int n = 2; n <= N; n++) {
            s = (s + cayley_mod(n, MOD)) % MOD;
        }
        cout << "sum_{n=2}^{" << N << "} n^(n-2) mod p = " << s << endl;
    }

    // Growth rate
    cout << "\n=== Growth Rate ===" << endl;
    for (int n = 2; n <= 10; n++) {
        double ratio = pow(n + 1, n - 1) / pow(n, n - 2);
        cout << "t(" << n + 1 << ")/t(" << n << ") = " << fixed
             << setprecision(2) << ratio << endl;
    }

    ll ans = 0;
    for (int n = 2; n <= 1000; n++)
        ans = (ans + cayley_mod(n, MOD)) % MOD;
    cout << "\nAnswer: " << ans << endl;

    return 0;
}
