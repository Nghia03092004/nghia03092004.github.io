#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 447: Retractions B
 *
 * R(n) = number of affine retractions f(x) = ax + b (mod n), i.e.,
 *        pairs (a,b) with a^2 ≡ a (mod n) and ab ≡ 0 (mod n).
 *
 * Key formula: R(n) = prod_{p^e || n} (1 + p^e).
 * R is multiplicative, computed via linear sieve.
 *
 * Answer: sum_{n=2}^{10^7} R(n) mod (10^9 + 7).
 */

const long long MOD = 1e9 + 7;
const int N = 10000000;

int spf[N + 1];         // smallest prime factor
long long R_val[N + 1]; // R(n) mod MOD

void compute_spf() {
    for (int i = 0; i <= N; i++) spf[i] = i;
    for (int i = 2; (long long)i * i <= N; i++) {
        if (spf[i] == i) { // i is prime
            for (int j = i * i; j <= N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

int main() {
    compute_spf();

    R_val[1] = 1;
    long long total = 0;

    for (int n = 2; n <= N; n++) {
        int p = spf[n];
        // Extract p^e from n
        int m = n;
        long long pe = 1;
        while (m % p == 0) {
            m /= p;
            pe *= p;
        }
        // R(n) = R(m) * (1 + p^e) mod MOD
        R_val[n] = R_val[m] % MOD * ((1 + pe) % MOD) % MOD;
        total = (total + R_val[n]) % MOD;
    }

    cout << total << endl;

    // Verification for small cases
    // R(2) = 3, R(3) = 4, R(4) = 5, R(6) = 12
    assert(R_val[2] == 3);
    assert(R_val[3] == 4);
    assert(R_val[4] == 5);
    assert(R_val[6] == 12);
    assert(R_val[12] == 20);
    assert(R_val[30] == 72);

    return 0;
}
