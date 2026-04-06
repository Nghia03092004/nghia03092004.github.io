#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;
const long long N = 100000000000000LL; // 10^14
const int L = 10000000; // sqrt(N) = 10^7

long long j2[L + 1]; // Jordan totient J_2
bool is_composite[L + 1];
vector<int> primes;

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
    // Initialize J_2(e) = e^2 mod MOD
    for (int i = 1; i <= L; i++) {
        j2[i] = (1LL * i % MOD) * (i % MOD) % MOD;
    }

    // Sieve: for each prime p, multiply J_2(m) by (1 - 1/p^2) = (p^2-1)/p^2
    // We need modular inverse of p^2
    for (int p = 2; p <= L; p++) {
        if (!is_composite[p]) {
            primes.push_back(p);
            long long p2 = (1LL * p * p) % MOD;
            long long factor = (p2 - 1 + MOD) % MOD * power(p2, MOD - 2, MOD) % MOD;
            for (long long m = p; m <= L; m += p) {
                is_composite[m] = (m != p);
                j2[m] = j2[m] * factor % MOD;
            }
        } else {
            // Mark composites for sieve
        }
    }

    // Actually we need a proper sieve. Let me redo using smallest prime factor.
    // Reset and use proper Euler sieve approach
    // The above loop already handles it: for each prime p, we iterate through
    // all multiples and multiply by (p^2-1)/p^2. This correctly computes J_2
    // since J_2 is multiplicative and J_2(n) = n^2 * prod_{p|n} (1 - 1/p^2).

    // Compute S(N) = sum_{e=1}^{L} J_2(e) * floor(N/e^2) mod MOD
    long long ans = 0;
    for (int e = 1; e <= L; e++) {
        long long q = (N / ((long long)e * e)) % MOD;
        ans = (ans + j2[e] * q) % MOD;
    }

    cout << ans << endl;
    return 0;
}
