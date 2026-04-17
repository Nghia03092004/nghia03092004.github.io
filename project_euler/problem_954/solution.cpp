/*
 * Problem 954: Prime Counting Function
 *
 * Compute sum_{k=1}^{10^4} pi(k^2).
 *
 * Algorithm:
 *   1. Sieve primes up to (10^4)^2 = 10^8
 *   2. Build prefix sum array of prime indicator
 *   3. Sum pi(k^2) for k = 1 to 10^4
 *
 * Complexity: O(M log log M), M = 10^8.
 * Memory: ~100 MB for boolean sieve + prefix sums.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 10000;
    const int M = N * N;  // 10^8

    // Sieve of Eratosthenes
    vector<bool> is_prime(M + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= M; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= M; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // Build prefix sum: pi[n] = number of primes <= n
    vector<int> pi(M + 1, 0);
    for (int n = 1; n <= M; n++) {
        pi[n] = pi[n - 1] + (is_prime[n] ? 1 : 0);
    }

    // Sum pi(k^2) for k = 1 to N
    long long total = 0;
    for (int k = 1; k <= N; k++) {
        total += pi[(long long)k * k];
    }

    cout << total << endl;
    return 0;
}
