#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 866: Tidying Up B
 *
 * A caterpillar of N=100 pieces is assembled randomly. When a segment of
 * length k is formed, H_k = k*(2k-1) is recorded. We want the expected
 * value of the product of all recorded hexagonal numbers, mod 987654319.
 *
 * Key insight: Consider which piece is placed LAST in each interval [a,b].
 * If piece at position i is placed last in interval [a,b] of length L=b-a+1,
 * then it contributes H_L to the product. The sub-intervals [a,i-1] and
 * [i+1,b] are filled independently.
 *
 * E(L) = H_L / L * sum_{i=0}^{L-1} E(i) * E(L-1-i)
 * E(0) = 1
 */

const long long MOD = 987654319;

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

long long inv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

int main() {
    const int N = 100;

    // H_k = k * (2k - 1)
    auto H = [&](long long k) -> long long {
        return k % MOD * ((2 * k - 1) % MOD) % MOD;
    };

    // E[L] = expected product for interval of length L
    vector<long long> E(N + 1, 0);
    E[0] = 1;

    for (int L = 1; L <= N; L++) {
        long long s = 0;
        for (int i = 0; i < L; i++) {
            s = (s + E[i] % MOD * E[L - 1 - i]) % MOD;
        }
        E[L] = H(L) % MOD * s % MOD * inv(L, MOD) % MOD;
    }

    cout << E[N] << endl;

    return 0;
}
