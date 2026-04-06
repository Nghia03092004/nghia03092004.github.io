/*
 * Project Euler Problem 452 — Long Products
 *
 * F(m,n) = number of n-tuples (a1,...,an) with ai >= 1 and product <= m.
 * Compute F(10^9, 10^9) mod 1234567891.
 *
 * F(m,n) = sum_{k=1}^{m} d_n(k) where d_n is the n-th Piltz divisor function.
 * d_n is multiplicative: d_n(p^a) = C(n+a-1, a).
 *
 * Algorithm:
 *   - Maintain partial sums S[v] for O(sqrt(m)) bucket values v = floor(m/k).
 *   - Process small primes (p <= sqrt(m)) with full exponent handling.
 *   - Process large primes (p > sqrt(m)) via segmented sieve, a=1 only.
 *
 * Compile: g++ -O2 -o solution solution_final.cpp
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;
typedef long long ll;

const ll MOD = 1234567891LL;

ll power(ll base, ll exp, ll m) {
    ll r = 1; base %= m;
    while (exp > 0) {
        if (exp & 1) r = r * base % m;
        base = base * base % m;
        exp >>= 1;
    }
    return r;
}

ll modinv(ll a, ll m) { return power(a, m - 2, m); }

ll binom_mod(ll n_val, int a) {
    if (a == 0) return 1;
    ll num = 1, den = 1;
    for (int i = 1; i <= a; i++) {
        num = num * ((n_val + a - i) % MOD) % MOD;
        den = den * ((ll)i % MOD) % MOD;
    }
    return num * modinv(den, MOD) % MOD;
}

ll solve(ll m, ll n) {
    if (m <= 0) return 0;
    if (m == 1) return 1;

    int sqm = (int)sqrt((double)m);
    while ((ll)(sqm + 1) * (sqm + 1) <= m) sqm++;
    int sz = sqm + 1;

    vector<ll> lo(sz, 0), hi(sz, 0);
    for (int i = 1; i < sz; i++) { lo[i] = 1; hi[i] = 1; }

    auto getS = [&](ll v) -> ll {
        return (v <= sqm) ? lo[(int)v] : hi[(int)(m / v)];
    };

    int max_a = (m >= 2) ? (int)(log((double)m) / log(2.0)) + 2 : 1;
    vector<ll> bc(max_a + 1);
    for (int a = 0; a <= max_a; a++) bc[a] = binom_mod(n, a);

    // Sieve small primes
    vector<int> small_primes;
    {
        vector<bool> ip(sqm + 1, true);
        ip[0] = ip[1] = false;
        for (int i = 2; (ll)i * i <= sqm; i++)
            if (ip[i])
                for (int j = i * i; j <= sqm; j += i)
                    ip[j] = false;
        for (int i = 2; i <= sqm; i++)
            if (ip[i]) small_primes.push_back(i);
    }

    printf("  sqm=%d, small primes: %zu\n", sqm, small_primes.size());

    // Step 1: Process small primes
    for (int p : small_primes) {
        // hi buckets (v > sqm), processed in decreasing v order (increasing k)
        for (int k = 1; k < sz; k++) {
            ll v = m / k;
            if (v <= sqm) break;
            if (v < p) break;
            ll pk = p; int a = 1;
            while (pk <= v) {
                hi[k] = (hi[k] + bc[a] * getS(v / pk)) % MOD;
                if (pk > v / p) break;
                pk *= p; a++;
            }
        }
        // lo buckets
        for (int v = sqm; v >= p; v--) {
            ll pk = p; int a = 1;
            while (pk <= v) {
                lo[v] = (lo[v] + bc[a] * getS(v / pk)) % MOD;
                if (pk > v / p) break;
                pk *= p; a++;
            }
        }
    }

    // Step 2: Process large primes via segmented sieve
    // For p > sqm: only a=1. Only hi buckets affected.
    // Process primes in DECREASING order within each segment.
    ll bc1 = bc[1];
    int seg_size = 1 << 19;  // 512K
    vector<bool> seg(seg_size);

    ll total_large = 0;
    for (ll seg_lo = (ll)sqm + 1; seg_lo <= m; seg_lo += seg_size) {
        ll seg_hi_val = min(seg_lo + seg_size - 1, m);
        int seg_len = (int)(seg_hi_val - seg_lo + 1);
        fill(seg.begin(), seg.begin() + seg_len, true);

        for (int p : small_primes) {
            ll start = ((seg_lo + p - 1) / p) * p;
            if (start < (ll)p * p) start = (ll)p * p;  // composites only
            for (ll j = start; j <= seg_hi_val; j += p)
                seg[(int)(j - seg_lo)] = false;
        }

        // Process primes in decreasing order
        for (int i = seg_len - 1; i >= 0; i--) {
            if (!seg[i]) continue;
            ll p = seg_lo + i;
            total_large++;
            for (int k = 1; k < sz && m / k >= p; k++) {
                ll vp = (m / k) / p;
                hi[k] = (hi[k] + bc1 * lo[(int)vp]) % MOD;
            }
        }
    }

    printf("  large primes processed: %lld\n", total_large);
    return (getS(m) % MOD + MOD) % MOD;
}

int main() {
    printf("=== Project Euler #452: Long Products ===\n\n");

    // Verification
    struct { ll m; ll n; ll expected; } tests[] = {
        {10, 10, 571},
        {100, 100, 613658361},
        {1000, 1000, 1229737624},
        {10000, 10000, 1027202788},
        {100000, 100000, 602287876},
    };
    printf("Verification:\n");
    bool all_ok = true;
    for (auto& t : tests) {
        ll r = solve(t.m, t.n);
        bool ok = (r == t.expected);
        if (!ok) all_ok = false;
        printf("  F(%lld, %lld) = %lld  (expected %lld) [%s]\n",
               t.m, t.n, r, t.expected, ok ? "OK" : "FAIL");
    }
    printf(all_ok ? "All tests passed.\n\n" : "SOME TESTS FAILED!\n\n");

    // Main computation
    printf("Computing F(10^9, 10^9) mod %lld ...\n", MOD);
    clock_t t0 = clock();
    ll ans = solve(1000000000LL, 1000000000LL);
    double elapsed = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("\n*** ANSWER: F(10^9, 10^9) mod %lld = %lld ***\n", MOD, ans);
    printf("Time: %.2f seconds\n", elapsed);

    return 0;
}
