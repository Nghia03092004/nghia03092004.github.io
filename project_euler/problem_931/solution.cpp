#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 931: Sum of Radical Expressions
 *
 * R(n) = sum_{d|n} rad(d). Find sum_{n=1}^{10^7} R(n) mod 10^9+7.
 *
 * rad(n) = product of distinct prime factors.
 * R is multiplicative: R(p^a) = 1 + a*p.
 * R(n) = prod_{p^a || n} (1 + a*p).
 *
 * Alternative: sum R(n) = sum_{d=1}^N rad(d) * floor(N/d).
 *
 * Algorithm: sieve to compute rad(d) for all d, then compute sum.
 */

int main() {
    const int N = 10000000;
    const long long MOD = 1000000007;
    
    // Compute rad(n) via sieve
    vector<long long> rad(N + 1, 1);
    for (int p = 2; p <= N; p++) {
        if (rad[p] == 1) { // p is prime
            for (int m = p; m <= N; m += p) {
                rad[m] *= p;
            }
        }
    }
    
    // sum R(n) = sum_{d=1}^N rad(d) * floor(N/d)
    long long total = 0;
    for (int d = 1; d <= N; d++) {
        total = (total + rad[d] % MOD * (N / d) % MOD) % MOD;
    }
    cout << total << endl;

    // Verify: R(6) = rad(1)+rad(2)+rad(3)+rad(6) = 1+2+3+6 = 12
    assert(rad[1] == 1 && rad[2] == 2 && rad[3] == 3 && rad[6] == 6);

    return 0;
}
