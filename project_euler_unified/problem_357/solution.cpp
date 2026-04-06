#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 357: Prime Generating Integers
 *
 * Find the sum of all n <= 10^8 such that for every divisor d of n,
 * d + n/d is prime.
 *
 * Approach:
 * - n+1 must be prime (d=1 test), so only check n = p-1 for primes p.
 * - n must be 1 or even.
 * - For each candidate, check all divisors d <= sqrt(n).
 *
 * Answer: 1739023853137
 */

const int LIMIT = 100000001;

vector<bool> sieve;

void build_sieve() {
    sieve.assign(LIMIT + 1, true);
    sieve[0] = sieve[1] = false;
    for (long long i = 2; i * i <= LIMIT; i++) {
        if (sieve[i]) {
            for (long long j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;
        }
    }
}

bool check(int n) {
    if (n == 1) return true;
    if (n % 2 != 0) return false;

    for (long long d = 2; (long long)d * d <= n; d++) {
        if (n % d == 0) {
            long long val = d + n / d;
            if (val > LIMIT || !sieve[val])
                return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    build_sieve();

    long long ans = 0;
    // n+1 must be prime, so iterate over primes p and set n = p-1
    for (int p = 2; p <= LIMIT; p++) {
        if (sieve[p]) {
            int n = p - 1;
            if (check(n)) {
                ans += n;
            }
        }
    }

    cout << ans << endl;
    return 0;
}
