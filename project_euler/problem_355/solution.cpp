#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 355: Maximal Coprime Subset
 *
 * S(n) = 1 + sum over primes p <= n of p^floor(log_p(n))
 *
 * We need sum_{n=1}^{N} S(n).
 *
 * For each prime p and exponent k, p^k contributes to S(n) for n in [p^k, p^{k+1}-1].
 * Specifically, for n >= p, the contribution of prime p is the largest p^a <= n.
 *
 * sum_{n=1}^{N} S(n) = N (from the 1's) +
 *   sum over primes p <= N, sum over k>=1 where p^k <= N:
 *     p^k * (min(p^{k+1}-1, N) - p^k + 1)
 *   + sum over primes p <= N:
 *     p * (min(p^2-1, N) - p + 1)  [k=1 contribution, but p^1 = p contributes for n in [p, p^2-1]]
 *
 * Wait, let me reconsider. For each prime p <= N:
 *   For n in [p, p^2-1] (if p^2-1 <= N, else [p, N]): contribution is p
 *   For n in [p^2, p^3-1]: contribution is p^2
 *   ...
 *   For n in [p^k, min(p^{k+1}-1, N)]: contribution is p^k
 *
 * Total contribution of prime p:
 *   sum_{k=1}^{...} p^k * (min(p^{k+1}-1, N) - p^k + 1)
 */

int main() {
    const long long N = 100000; // 10^5

    // Sieve primes up to N
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (long long i = 2; i <= N; i++) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= N; j += i)
                is_prime[j] = false;
        }
    }

    long long total = N; // contribution of 1 for each n from 1 to N

    for (long long p = 2; p <= N; p++) {
        if (!is_prime[p]) continue;

        long long pk = p; // p^k, starting with k=1
        while (pk <= N) {
            long long next_pk = pk * p; // p^{k+1}
            long long upper;
            if (next_pk - 1 <= N && next_pk > pk) { // check no overflow
                upper = next_pk - 1;
            } else {
                upper = N;
            }
            // pk contributes for n in [pk, upper]
            long long count = upper - pk + 1;
            total += pk * count;

            if (pk > N / p) break; // prevent overflow
            pk *= p;
        }
    }

    cout << total << endl;

    return 0;
}
