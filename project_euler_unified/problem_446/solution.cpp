#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 446: Retractions B
 *
 * R is multiplicative: R(p^k) = p^(k-1) * (p+1)
 * n^4 + 4 = (n^2 - 2n + 2)(n^2 + 2n + 2) = P * Q
 * P = (n-1)^2 + 1, Q = (n+1)^2 + 1
 * Max value of P or Q ~ 10^14, so sqrt ~ 10^7
 * We sieve primes up to 10^7 and use them for trial division.
 */

const long long MOD = 1000000007;

long long power(long long base, int exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int N = 10000000;
    const int PLIMIT = 10000010; // primes up to 10^7

    // Sieve primes up to 10^7
    vector<int> primes;
    vector<bool> sieve(PLIMIT, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i < PLIMIT; i++) {
        if (sieve[i]) {
            primes.push_back(i);
            if ((long long)i * i < PLIMIT)
                for (int j = i * i; j < PLIMIT; j += i)
                    sieve[j] = false;
        }
    }

    // For factorizing a number up to ~10^14, trial division by primes up to 10^7
    // Each prime factor p of val satisfies p <= sqrt(val) ~ 10^7 or val is prime

    long long ans = 0;

    for (long long n = 1; n <= N; n++) {
        long long P = n * n - 2 * n + 2;
        long long Q = n * n + 2 * n + 2;

        // We compute R(P*Q) by factorizing P and Q together
        // Store factors as (prime, exponent) pairs
        long long r = 1;

        // Small buffer for factors from P that might also appear in Q
        // We'll factorize P first, collect factors, then factorize Q
        // and merge on the fly

        // Actually, let's store P's factors and then process Q
        static long long fprimes[30];
        static int fexps[30];
        int fcnt = 0;

        // Factorize P
        long long tmp = P;
        for (int idx = 0; idx < (int)primes.size(); idx++) {
            long long p = primes[idx];
            if (p * p > tmp) break;
            if (tmp % p == 0) {
                fprimes[fcnt] = p;
                fexps[fcnt] = 0;
                while (tmp % p == 0) { tmp /= p; fexps[fcnt]++; }
                fcnt++;
            }
        }
        if (tmp > 1) { fprimes[fcnt] = tmp; fexps[fcnt] = 1; fcnt++; }

        // Now factorize Q, merging with P's factors
        tmp = Q;
        // Check P's prime factors in Q first
        for (int i = 0; i < fcnt; i++) {
            while (tmp % fprimes[i] == 0) {
                tmp /= fprimes[i];
                fexps[i]++;
            }
        }
        // Now remaining factors of Q
        int fcnt2_start = fcnt;
        for (int idx = 0; idx < (int)primes.size(); idx++) {
            long long p = primes[idx];
            if (p * p > tmp) break;
            if (tmp % p == 0) {
                fprimes[fcnt] = p;
                fexps[fcnt] = 0;
                while (tmp % p == 0) { tmp /= p; fexps[fcnt]++; }
                fcnt++;
            }
        }
        if (tmp > 1) { fprimes[fcnt] = tmp; fexps[fcnt] = 1; fcnt++; }

        // Compute R
        for (int i = 0; i < fcnt; i++) {
            long long pp = fprimes[i] % MOD;
            int k = fexps[i];
            long long contrib = power(fprimes[i], k - 1, MOD) * ((pp + 1) % MOD) % MOD;
            r = r * contrib % MOD;
        }

        ans = (ans + r) % MOD;
    }

    cout << ans << endl;
    return 0;
}
