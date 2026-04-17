#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 417: Reciprocal Cycles II
 *
 * Find sum of L(n) for 3 <= n <= 10^8, where L(n) is the
 * recurring cycle length of 1/n.
 *
 * L(n) = multiplicative order of 10 mod n' (n with factors of 2,5 removed).
 *
 * Answer: 446572970925740
 */

const int N = 100000000;

int spf[N + 1]; // smallest prime factor

void sieve() {
    for (int i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (int j = i; j <= N; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Compute multiplicative order of 10 mod p (p is prime, p != 2, 5)
long long ord10(long long p) {
    long long d = p - 1;
    // factorize d
    long long temp = d;
    vector<long long> factors;
    while (temp > 1) {
        long long f;
        if (temp <= N) {
            f = spf[temp];
        } else {
            f = 0;
            for (long long i = 2; i * i <= temp; i++) {
                if (temp % i == 0) { f = i; break; }
            }
            if (f == 0) f = temp;
        }
        factors.push_back(f);
        while (temp % f == 0) temp /= f;
    }

    for (long long f : factors) {
        while (d % f == 0 && power_mod(10, d / f, p) == 1) {
            d /= f;
        }
    }
    return d;
}

int main() {
    sieve();

    // Precompute ord10 for each prime
    // Then for each n, compute L(n)
    // L(n) depends on n' = n with factors 2,5 removed
    // For n' = p1^a1 * p2^a2 * ..., L(n) = lcm(ord10(p1)*p1^(a1-1), ...)

    // Store ord for primes
    vector<long long> prime_ord(N + 1, 0);
    for (int p = 3; p <= N; p++) {
        if (spf[p] == p && p != 5) {
            prime_ord[p] = ord10(p);
        }
    }

    long long total = 0;

    for (int n = 3; n <= N; n++) {
        int m = n;
        // Remove factors of 2 and 5
        while (m % 2 == 0) m /= 2;
        while (m % 5 == 0) m /= 5;
        if (m <= 1) continue;

        // Factorize m and compute L(n) = lcm of ord10(p)*p^(k-1) for each p^k
        long long L = 1;
        int temp = m;
        while (temp > 1) {
            int p = spf[temp];
            int k = 0;
            while (temp % p == 0) {
                temp /= p;
                k++;
            }
            long long contrib = prime_ord[p];
            for (int i = 1; i < k; i++) contrib *= p;
            L = L / __gcd(L, contrib) * contrib;
        }
        total += L;
    }

    cout << total << endl;
    return 0;
}
