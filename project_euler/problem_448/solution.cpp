#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 448: Average Least Common Multiple
 *
 * f(n) = (1/n) * sum_{k=1}^{n} lcm(k, n)
 *       = (1 + sum_{d|n} d*phi(d)) / 2
 *
 * S(N) = sum_{n=1}^{N} f(n) mod (10^9 + 7).
 *
 * Algorithm:
 *   1. Sieve phi(n) for n = 1..N.
 *   2. Compute h(n) = n * phi(n) mod p.
 *   3. Compute g(n) = sum_{d|n} h(d) mod p via additive sieve.
 *   4. f(n) = (1 + g(n)) / 2 mod p.
 *   5. Sum all f(n).
 */

const long long MOD = 1e9 + 7;
const long long INV2 = (MOD + 1) / 2;  // modular inverse of 2
const int N = 100000000;  // 10^8

int phi_arr[N + 1];
long long g[N + 1];  // sum_{d|n} d*phi(d) mod MOD

int main() {
    // Step 1: Euler totient sieve
    for (int i = 0; i <= N; i++) phi_arr[i] = i;
    for (int i = 2; i <= N; i++) {
        if (phi_arr[i] == i) {  // i is prime
            for (int j = i; j <= N; j += i) {
                phi_arr[j] = phi_arr[j] / i * (i - 1);
            }
        }
    }

    // Steps 2-3: Compute g(n) = sum_{d|n} d*phi(d)
    memset(g, 0, sizeof(g));
    for (int d = 1; d <= N; d++) {
        long long hd = (long long)d % MOD * (phi_arr[d] % MOD) % MOD;
        for (int multiple = d; multiple <= N; multiple += d) {
            g[multiple] = (g[multiple] + hd) % MOD;
        }
    }

    // Steps 4-5: f(n) = (1 + g(n)) * inv2, sum all
    long long total = 0;
    for (int n = 1; n <= N; n++) {
        long long fn = (1 + g[n]) % MOD * INV2 % MOD;
        total = (total + fn) % MOD;
    }

    cout << total << endl;

    // Verification
    assert(total == 106467648LL);

    return 0;
}
