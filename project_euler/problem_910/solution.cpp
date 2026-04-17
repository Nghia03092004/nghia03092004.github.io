#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 910: Prime Constellation Counting
 *
 * Count twin prime pairs (p, p+2) with p < 10^7.
 *
 * Method: Sieve of Eratosthenes + linear scan.
 *
 * Hardy-Littlewood conjecture: pi_2(N) ~ 2*C_2 * N / (ln N)^2
 * where C_2 = prod_{p>=3} p(p-2)/(p-1)^2 ≈ 0.6602.
 *
 * Brun's theorem: sum of 1/p over twin primes converges (B ≈ 1.9022).
 */

int main() {
    const int N = 10000000;

    // Sieve of Eratosthenes
    vector<bool> sieve(N + 3, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; (long long)i * i <= N + 2; i++) {
        if (sieve[i]) {
            for (int j = i * i; j <= N + 2; j += i) {
                sieve[j] = false;
            }
        }
    }

    // Count twin prime pairs
    int count = 0;
    for (int p = 2; p <= N; p++) {
        if (sieve[p] && sieve[p + 2]) {
            count++;
        }
    }

    // Verify small known values
    // Twin primes below 100: (3,5),(5,7),(11,13),(17,19),(29,31),(41,43),(59,61),(71,73) = 8
    int small_count = 0;
    for (int p = 2; p <= 100; p++)
        if (sieve[p] && sieve[p + 2])
            small_count++;
    assert(small_count == 8);

    // Below 1000: 35 pairs
    int med_count = 0;
    for (int p = 2; p <= 1000; p++)
        if (sieve[p] && sieve[p + 2])
            med_count++;
    assert(med_count == 35);

    cout << count << endl;
    return 0;
}
