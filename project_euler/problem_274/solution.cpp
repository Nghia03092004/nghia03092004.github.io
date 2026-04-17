#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Problem 274: Divisibility Multipliers
//
// For each prime p coprime to 10 (i.e., p > 5 and p != 2),
// the divisibility multiplier m(p) is the modular inverse of 10 mod p.
// Find sum of m(p) for all primes 5 < p < 10^7.

int main() {
    const int LIMIT = 10000000;

    // Sieve of Eratosthenes
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (ll)i * i < LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    ll total = 0;
    for (int p = 3; p < LIMIT; p++) {
        if (p == 5) continue;
        if (!is_prime[p]) continue;
        // Compute 10^{-1} mod p using Fermat's little theorem: 10^{p-2} mod p
        // Or use extended GCD
        ll inv = 1;
        ll base = 10 % p;
        ll exp = p - 2;
        ll mod = p;
        ll b = base;
        ll e = exp;
        ll r = 1;
        while (e > 0) {
            if (e & 1) r = r * b % mod;
            b = b * b % mod;
            e >>= 1;
        }
        total += r;
    }

    cout << total << endl;
    return 0;
}
