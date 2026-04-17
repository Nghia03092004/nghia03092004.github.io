#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 929: Prime Gap Distribution
 *
 * Find sum_{p <= 10^7} g(p)^2 where g(p) = next_prime - p.
 *
 * Average gap ~ ln(p) ~ 16 near 10^7.
 * Max gap below 10^7: ~154.
 * Heuristic: sum g^2 ~ 2N ln N.
 *
 * Cramer's conjecture: g_n = O((ln p_n)^2).
 *
 * Algorithm: sieve primes to N+1000, iterate consecutive pairs.
 * Complexity: O(N log log N) sieve.
 */

int main() {
    const int N = 10000000;
    const int LIM = N + 1000;
    vector<bool> sieve(LIM + 1, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; (long long)i * i <= LIM; i++)
        if (sieve[i])
            for (int j = i * i; j <= LIM; j += i)
                sieve[j] = false;

    long long total = 0;
    int prev = 2;
    int max_gap = 0;
    for (int i = 3; i <= LIM; i++) {
        if (sieve[i]) {
            if (prev <= N) {
                long long g = i - prev;
                total += g * g;
                if (g > max_gap) max_gap = g;
            }
            prev = i;
        }
    }
    cout << total << endl;
    // cerr << "Max gap: " << max_gap << endl;

    return 0;
}
