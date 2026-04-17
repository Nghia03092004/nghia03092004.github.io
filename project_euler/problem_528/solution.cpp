#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 528: Constrained Sums
 *
 * S(n,k,b) = number of solutions to x1+x2+...+xk <= n with 0 <= xm <= b^m.
 * Find (sum_{k=10}^{15} S(10^k, k, k)) mod 10^9+7.
 *
 * Uses inclusion-exclusion with modular arithmetic.
 */

typedef long long ll;
typedef __int128 lll;

const ll MOD = 1000000007;

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

ll mod_inv(ll a, ll mod) {
    return power(a, mod - 2, mod);
}

// Compute C(N, k) mod p where N can be very large but k is small
// C(N, k) = N*(N-1)*...*(N-k+1) / k!
ll binom_large_n(ll N, int k) {
    if (N < 0 || N < k) return 0;
    if (k == 0) return 1;

    // Compute numerator mod MOD
    ll num = 1;
    for (int i = 0; i < k; i++) {
        ll term = ((N - i) % MOD + MOD) % MOD;
        num = num * term % MOD;
    }

    // Compute k! mod MOD
    ll den = 1;
    for (int i = 1; i <= k; i++) {
        den = den * i % MOD;
    }

    return num % MOD * mod_inv(den, MOD) % MOD;
}

// Compute S(n, k, b) mod MOD using inclusion-exclusion
// The upper bounds are b^1, b^2, ..., b^k
// We need subsets T of {1,...,k} where sum of (b^m + 1) for m in T <= n
ll compute_S(ll n, int k, int b) {
    // Precompute b^m for m = 1..k (as regular integers, might overflow for large b,k)
    // For b=k<=15, b^k can be up to 15^15 ~ 4.37e17, fits in long long
    vector<ll> bpow(k + 1);
    bpow[0] = 1;
    for (int m = 1; m <= k; m++) {
        // Check overflow
        if (bpow[m-1] > 1e18 / b) {
            bpow[m] = (ll)2e18; // sentinel: too large
        } else {
            bpow[m] = bpow[m-1] * b;
        }
    }

    ll result = 0;

    // Enumerate subsets of {1,...,k} using bitmask
    for (int mask = 0; mask < (1 << k); mask++) {
        int bits = __builtin_popcount(mask);
        ll subtract = 0;
        bool overflow = false;

        for (int m = 0; m < k; m++) {
            if (mask & (1 << m)) {
                ll val = bpow[m + 1] + 1;
                if (subtract > 1e18 - val) {
                    overflow = true;
                    break;
                }
                subtract += val;
            }
        }

        if (overflow || subtract > n) continue;

        ll remaining = n - subtract;
        ll coeff = binom_large_n(remaining + k, k);

        if (bits % 2 == 0) {
            result = (result + coeff) % MOD;
        } else {
            result = (result - coeff + MOD) % MOD;
        }
    }

    return result;
}

int main() {
    // Verify small cases
    cout << "S(14,3,2) = " << compute_S(14, 3, 2) << endl;      // 135
    cout << "S(200,5,3) = " << compute_S(200, 5, 3) << endl;    // 12949440

    ll total = 0;
    for (int k = 10; k <= 15; k++) {
        // n = 10^k
        ll n = 1;
        for (int i = 0; i < k; i++) n *= 10;

        ll s = compute_S(n, k, k);
        cout << "S(10^" << k << "," << k << "," << k << ") mod 10^9+7 = " << s << endl;
        total = (total + s) % MOD;
    }

    cout << "Answer: " << total << endl;

    return 0;
}
