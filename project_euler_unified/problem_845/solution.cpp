#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 845: Prime Pairs
 *
 * Count twin primes (p, p+2) with p <= N.
 * Uses sieve of Eratosthenes.
 */

const int MAXN = 10000001;
bool sieve[MAXN + 2];

int count_twin_primes(int N) {
    fill(sieve, sieve + N + 3, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; (long long)i * i <= N + 2; i++) {
        if (sieve[i]) {
            for (int j = i * i; j <= N + 2; j += i)
                sieve[j] = false;
        }
    }
    int count = 0;
    for (int p = 2; p <= N; p++) {
        if (sieve[p] && sieve[p + 2])
            count++;
    }
    return count;
}

int main() {
    // Verify known values
    assert(count_twin_primes(100) == 8);
    assert(count_twin_primes(1000) == 35);
    assert(count_twin_primes(10000) == 205);

    int ans = count_twin_primes(10000000);
    cout << ans << endl;  // 58980
    return 0;
}
