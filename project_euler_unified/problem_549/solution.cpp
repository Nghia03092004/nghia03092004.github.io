#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 549: Divisibility of Factorials
 * Find S(10^8) where s(n) = smallest m such that n | m!
 * Key: s(n) = max over prime power factors p^e of s(p^e)
 * Use sieve to compute s(n) for all n up to N.
 */

const int N = 100000000; // 10^8

// s[i] = smallest m such that i | m!
// We store as short values relative approach: use int array
static int s[N + 1];

// Compute the p-adic valuation of m!
// i.e., the exponent of p in m!
long long factorial_val(int m, int p) {
    long long v = 0;
    long long pk = p;
    while (pk <= m) {
        v += m / pk;
        if (pk > m / p) break; // prevent overflow
        pk *= p;
    }
    return v;
}

// Find smallest m such that p^e | m!
// m must be a multiple of p
int find_s_prime_power(int p, int e) {
    // Binary search: find smallest m (multiple of p) with factorial_val(m, p) >= e
    // Lower bound: e (since val(e!) >= e/p + ... but we need at least e)
    // Actually s(p^e) <= p * e (since val((pe)!) >= e + e/p + ... >= e)
    // But we can do binary search on multiples of p

    int lo = p, hi = (long long)p * e < N ? p * e : N;
    // Ensure hi is multiple of p
    hi = (hi / p) * p;
    if (hi < p) hi = p;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        mid = (mid / p) * p;
        if (mid < lo) mid = lo;
        if (factorial_val(mid, p) >= e) {
            hi = mid;
        } else {
            mid += p;
            if (mid > hi) { lo = hi; break; }
            lo = mid;
        }
    }
    return lo;
}

int main() {
    ios_base::sync_with_stdio(false);

    // Initialize s array
    // s[0] = s[1] = 0 (not used)
    // For primes p, s[p] = p
    // For prime powers p^e, s[p^e] = find_s_prime_power(p, e)
    // For composite n, s[n] = max over prime power factors

    // Step 1: Sieve to find smallest prime factor
    vector<int> spf(N + 1, 0);
    for (int i = 2; i <= N; i++) {
        if (spf[i] == 0) { // i is prime
            for (int j = i; j <= N; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    // Step 2: For each prime, precompute s(p^e) for all powers p^e <= N
    // We'll store these in a map for quick lookup, but it's faster to
    // just compute s(n) by factoring n using spf

    // Step 3: Compute s(n) for each n from 2 to N
    long long total = 0;

    for (int n = 2; n <= N; n++) {
        int tmp = n;
        int result = 0;
        while (tmp > 1) {
            int p = spf[tmp];
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            // compute s(p^e)
            int sp = find_s_prime_power(p, e);
            result = max(result, sp);
        }
        total += result;
    }

    cout << total << endl;

    return 0;
}
