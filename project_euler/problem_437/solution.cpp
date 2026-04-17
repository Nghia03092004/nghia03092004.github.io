#include <bits/stdc++.h>
using namespace std;

// Project Euler 437: Fibonacci Primitive Roots
// Find sum of primes < 10^8 having at least one Fibonacci primitive root.

const int LIMIT = 100000000;

vector<int> spf; // smallest prime factor
vector<bool> is_prime_sieve;

void sieve() {
    spf.assign(LIMIT, 0);
    is_prime_sieve.assign(LIMIT, true);
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (int i = 2; i < LIMIT; i++) {
        if (is_prime_sieve[i]) {
            spf[i] = i;
            for (long long j = (long long)i * i; j < LIMIT; j += i) {
                is_prime_sieve[j] = false;
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

// Get distinct prime factors of n using SPF sieve
vector<int> prime_factors(int n) {
    vector<int> factors;
    while (n > 1) {
        int p = spf[n];
        factors.push_back(p);
        while (n % p == 0) n /= p;
    }
    return factors;
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

// Tonelli-Shanks algorithm: compute sqrt(n) mod p
long long tonelli_shanks(long long n, long long p) {
    if (n == 0) return 0;
    if (p == 2) return n & 1;

    // Check if n is QR
    if (power_mod(n, (p - 1) / 2, p) != 1) return -1;

    if (p % 4 == 3) {
        return power_mod(n, (p + 1) / 4, p);
    }

    // Factor out powers of 2 from p-1
    long long Q = p - 1, S = 0;
    while (Q % 2 == 0) { Q /= 2; S++; }

    // Find a non-residue
    long long z = 2;
    while (power_mod(z, (p - 1) / 2, p) != p - 1) z++;

    long long M = S;
    long long c = power_mod(z, Q, p);
    long long t = power_mod(n, Q, p);
    long long R = power_mod(n, (Q + 1) / 2, p);

    while (true) {
        if (t == 1) return R;
        long long i = 0, tmp = t;
        while (tmp != 1) { tmp = (__int128)tmp * tmp % p; i++; }
        long long b = c;
        for (long long j = 0; j < M - i - 1; j++)
            b = (__int128)b * b % p;
        M = i;
        c = (__int128)b * b % p;
        t = (__int128)t * c % p;
        R = (__int128)R * b % p;
    }
}

bool is_primitive_root(long long g, long long p, const vector<int>& factors) {
    if (g == 0) return false;
    for (int q : factors) {
        if (power_mod(g, (p - 1) / q, p) == 1)
            return false;
    }
    return true;
}

int main() {
    sieve();

    long long ans = 0;

    // p=5: check if it has a Fibonacci primitive root
    // g^2 - g - 1 = 0 mod 5: g^2 - g - 1, try g=1..4:
    // g=3: 9-3-1=5=0 mod 5, and 3 is primitive root of 5 (3^1=3,3^2=4,3^3=2,3^4=1)
    ans += 5;

    for (int p = 2; p < LIMIT; p++) {
        if (!is_prime_sieve[p]) continue;
        if (p <= 5) continue; // handled p=5 above, p=2,3 don't work

        // Check p ≡ ±1 (mod 5)
        int r = p % 5;
        if (r != 1 && r != 4) continue;

        // Compute sqrt(5) mod p
        long long sqrt5 = tonelli_shanks(5, p);
        if (sqrt5 < 0) continue;

        // inv(2) mod p
        long long inv2 = (p + 1) / 2;

        // Two candidates
        long long g1 = (__int128)(1 + sqrt5) * inv2 % p;
        long long g2 = (__int128)(1 + p - sqrt5) * inv2 % p;

        // Factor p-1
        vector<int> factors = prime_factors(p - 1);

        if (is_primitive_root(g1, p, factors) || is_primitive_root(g2, p, factors)) {
            ans += p;
        }
    }

    printf("%lld\n", ans);
    return 0;
}
