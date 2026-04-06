/*
 * Project Euler Problem 131: Prime Cube Partnership
 *
 * A prime p satisfies n^3 + n^2*p = k^3 iff p = (s+1)^3 - s^3
 * for some positive integer s. Count such primes below 10^6.
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000000;
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;

    int count = 0;
    for (long long s = 1; ; s++) {
        long long p = 3 * s * s + 3 * s + 1;
        if (p >= LIMIT) break;
        if (is_prime[p]) count++;
    }

    cout << count << endl;
    return 0;
}
