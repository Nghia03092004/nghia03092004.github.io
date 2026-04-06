#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 813: XOR Power
 *
 * XOR-product = carryless multiplication in GF(2)[x].
 * P(n) = n^{otimes n} mod (n^{otimes 2} XOR n XOR 1)
 * Sum P(n) for n = 2..N.
 *
 * Uses repeated squaring in GF(2)[x] quotient ring.
 */

const long long MOD = 1e9 + 7;

int deg(long long a) {
    if (a == 0) return -1;
    return 63 - __builtin_clzll(a);
}

long long xor_mul(long long a, long long b) {
    long long result = 0;
    while (b > 0) {
        if (b & 1) result ^= a;
        a <<= 1;
        b >>= 1;
    }
    return result;
}

long long xor_mod(long long a, long long m) {
    int dm = deg(m);
    while (deg(a) >= dm) {
        a ^= (m << (deg(a) - dm));
    }
    return a;
}

long long xor_mul_mod(long long a, long long b, long long m) {
    long long result = 0;
    a = xor_mod(a, m);
    while (b > 0) {
        if (b & 1) result ^= a;
        a <<= 1;
        if (deg(a) >= deg(m)) a ^= m;
        b >>= 1;
    }
    return result;
}

long long xor_pow_mod(long long base, long long exp, long long m) {
    if (m == 1) return 0;
    long long result = 1;
    base = xor_mod(base, m);
    while (exp > 0) {
        if (exp & 1) result = xor_mul_mod(result, base, m);
        base = xor_mul_mod(base, base, m);
        exp >>= 1;
    }
    return result;
}

long long P(long long n) {
    if (n < 2) return 0;
    long long n_sq = xor_mul(n, n);
    long long modulus = n_sq ^ n ^ 1;
    if (modulus == 0) return 0;
    return xor_pow_mod(n, n, modulus);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verify small cases
    assert(P(2) == 3);  // x^2 mod (x^2+x+1) = x+1 = 3

    long long N = 1000;
    long long total = 0;
    for (long long n = 2; n <= N; n++) {
        total = (total + P(n)) % MOD;
    }

    cout << total << endl;
    return 0;
}
