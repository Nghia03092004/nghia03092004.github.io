#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 819: Iterative Sampling
 *
 * Coupon collector / absorbing Markov chain.
 * E[T_n] = n * H_n = n * sum_{k=1}^{n} 1/k
 * Computed modulo 10^9+7 via modular inverses.
 *
 * Absorbing Markov chain: E[T] = (I-Q)^{-1} * 1
 */

const long long MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long modinv(long long a, long long mod = MOD) {
    return power(a % mod, mod - 2, mod);
}

// Compute H_n mod p = sum_{k=1}^{n} k^{-1} mod p
long long harmonic_mod(long long n, long long mod) {
    long long h = 0;
    for (long long k = 1; k <= n; k++) {
        h = (h + modinv(k, mod)) % mod;
    }
    return h;
}

// E[T_n] = n * H_n mod p
long long coupon_collector_mod(long long n, long long mod) {
    return n % mod * harmonic_mod(n, mod) % mod;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verify small cases
    // n=1: E[T] = 1
    assert(coupon_collector_mod(1, MOD) == 1);

    // n=2: E[T] = 3
    assert(coupon_collector_mod(2, MOD) == 3);

    // n=3: E[T] = 11/2 mod p = 11 * inv(2) mod p
    long long expected_3 = 11 * modinv(2) % MOD;
    assert(coupon_collector_mod(3, MOD) == expected_3);

    // n=4: E[T] = 25/3 mod p
    long long expected_4 = 25 * modinv(3) % MOD;
    assert(coupon_collector_mod(4, MOD) == expected_4);

    // Compute for large N
    long long N = 1000000;
    long long ans = coupon_collector_mod(N, MOD);
    cout << ans << endl;

    return 0;
}
