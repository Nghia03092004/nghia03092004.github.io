#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 921: Divisor Chain Lengths
 *
 * L(n) = max divisor chain length from n to 1 = Omega(n).
 * Find sum_{n=1}^{10^5} Omega(n) mod 10^9+7.
 *
 * Key insight: The longest chain from n to 1 via proper divisors
 * has length Omega(n) = sum of exponents in prime factorization.
 *
 * Proof:
 *   Upper bound: each step a_{i+1} | a_i, a_{i+1} < a_i implies
 *     a_i/a_{i+1} >= 2, so Omega decreases by >= 1 per step.
 *   Lower bound: divide by one prime at a time.
 *
 * Sieve approach: for each prime p, for each power p^k <= N,
 * add 1 to Omega(m) for all multiples m of p^k.
 *
 * Alternative formula: sum Omega(n) = sum_{p prime} sum_k floor(N/p^k).
 *
 * Complexity: O(N log log N) sieve, O(N) summation.
 */

int main() {
    const int N = 100000;
    const long long MOD = 1000000007;

    vector<int> omega(N + 1, 0);
    for (int p = 2; p <= N; p++) {
        if (omega[p] == 0) { // p is prime
            for (long long pk = p; pk <= N; pk *= p) {
                for (long long m = pk; m <= N; m += pk) {
                    omega[m]++;
                }
            }
        }
    }

    long long ans = 0;
    for (int i = 1; i <= N; i++) {
        ans += omega[i];
    }

    cout << ans % MOD << endl;

    // Verification
    assert(omega[1] == 0);
    assert(omega[6] == 2);   // 2 * 3
    assert(omega[8] == 3);   // 2^3
    assert(omega[12] == 3);  // 2^2 * 3
    assert(omega[60] == 4);  // 2^2 * 3 * 5
    assert(omega[64] == 6);  // 2^6

    return 0;
}
