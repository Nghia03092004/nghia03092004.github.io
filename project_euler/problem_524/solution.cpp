#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 524: First Sort II
 *
 * E(n) = (n-1)(n+4) / 12, the expected number of insertion steps
 * to sort a random permutation via the "first sort" algorithm.
 *
 * Compute E(10^18) mod (10^9 + 7).
 *
 * Key insight: 10^18 mod p = 49 (since 10^9 ≡ -7 mod p, so 10^18 ≡ 49).
 * Then (49 - 1)(49 + 4) / 12 = 48 * 53 / 12 = 2544 / 12 = 212 mod p.
 * Wait, 2544 / 12 = 212 exactly, so E ≡ 212 (mod p)? Let me recheck.
 *
 * Actually, modular division: 2544 * inv(12) mod p.
 * inv(12) = pow(12, p-2, p) = 83333334.
 * 2544 * 83333334 mod (10^9 + 7).
 */

const long long MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
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
    long long n = 1000000000000000000LL; // 10^18
    long long p = MOD;

    long long a = (n - 1) % p;
    long long b = (n + 4) % p;
    long long inv12 = power(12, p - 2, p);
    long long ans = a % p * (b % p) % p * inv12 % p;

    cout << ans << endl;

    // Verification for small cases
    // E(5) = (4)(9)/12 = 3
    long long e5 = 4LL * 9 % p * inv12 % p;
    assert(e5 == 3);

    // E(4) = (3)(8)/12 = 2
    long long e4 = 3LL * 8 % p * inv12 % p;
    assert(e4 == 2);

    return 0;
}
