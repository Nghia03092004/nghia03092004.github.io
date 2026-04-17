/*
 * Project Euler Problem 132: Large Repunit Factors
 *
 * A prime p (not 2, 3, 5) divides R(10^9) iff 10^(10^9) = 1 (mod p).
 * Find the sum of the first 40 such primes.
 */
#include <bits/stdc++.h>
using namespace std;

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

int main() {
    const int LIMIT = 200000;
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;

    long long K = 1000000000LL;
    long long total = 0;
    int count = 0;

    for (int p = 2; p < LIMIT && count < 40; p++) {
        if (!is_prime[p]) continue;
        if (p == 2 || p == 3 || p == 5) continue;
        if (power_mod(10, K, p) == 1) {
            total += p;
            count++;
        }
    }

    cout << total << endl;
    return 0;
}
