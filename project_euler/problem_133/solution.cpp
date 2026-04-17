/*
 * Project Euler Problem 133: Repunit Nonfactors
 *
 * Sum of all primes below 100000 that never divide R(10^n).
 * A prime p can divide some R(10^n) iff ord_p(10) is 5-smooth.
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
    const int LIMIT = 100000;
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;

    long long total = 0;

    for (int p = 2; p < LIMIT; p++) {
        if (!is_prime[p]) continue;
        if (p == 2 || p == 3 || p == 5) {
            total += p;
            continue;
        }

        // Compute ord_p(10) by factoring p-1 and refining
        int pm1 = p - 1;
        vector<int> factors;
        int temp = pm1;
        for (int f = 2; (long long)f * f <= temp; f++) {
            if (temp % f == 0) {
                factors.push_back(f);
                while (temp % f == 0) temp /= f;
            }
        }
        if (temp > 1) factors.push_back(temp);

        int d = pm1;
        for (int f : factors)
            while (d % f == 0 && power_mod(10, d / f, p) == 1)
                d /= f;

        // Check if d is 5-smooth
        int dd = d;
        while (dd % 2 == 0) dd /= 2;
        while (dd % 5 == 0) dd /= 5;

        if (dd != 1)
            total += p;
    }

    cout << total << endl;
    return 0;
}
