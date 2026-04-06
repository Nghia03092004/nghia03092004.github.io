#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 734: A Bit of Prime
 *
 * T(n, k) = number of k-tuples of primes with OR also prime.
 * Subset zeta transform + Mobius inversion on boolean lattice.
 */

const int MOD = 1e9 + 7;

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

int main() {
    int n = 1000000;
    long long k = 999983;

    // Sieve
    vector<bool> is_p(n + 1, true);
    is_p[0] = is_p[1] = false;
    for (int i = 2; (long long)i*i <= n; i++)
        if (is_p[i]) for (int j = i*i; j <= n; j += i) is_p[j] = false;

    int bits = 0;
    while ((1 << bits) <= n) bits++;
    int sz = 1 << bits;

    // f[m] = count of primes that are submasks of m
    vector<int> f(sz, 0);
    for (int p = 2; p <= n; p++)
        if (is_p[p]) f[p]++;

    // Zeta transform
    for (int i = 0; i < bits; i++)
        for (int m = 0; m < sz; m++)
            if (m & (1 << i)) f[m] += f[m ^ (1 << i)];

    // For each prime q, Mobius inversion
    long long total = 0;
    for (int q = 2; q <= n; q++) {
        if (!is_p[q]) continue;
        int qbits = __builtin_popcount(q);
        long long gq = 0;
        // Enumerate submasks
        for (int s = q; ; s = (s - 1) & q) {
            int sbits = __builtin_popcount(s);
            long long sign = ((qbits - sbits) & 1) ? MOD - 1 : 1;
            gq = (gq + sign % MOD * power(f[s], k, MOD)) % MOD;
            if (s == 0) break;
        }
        total = (total + gq) % MOD;
    }

    printf("%lld\n", total);
    return 0;
}
