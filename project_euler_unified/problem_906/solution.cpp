#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 906: Fibonacci Modular Properties
 *
 * Compute sum_{k=1}^{10^7} F_k mod (10^9+7).
 *
 * Key identity: sum_{k=1}^{n} F_k = F_{n+2} - 1  (telescoping)
 *
 * Two methods for computing F(n) mod p:
 *   1. Matrix exponentiation:  Q^n gives F(n+1), F(n)  in O(log n)
 *   2. Fast doubling formulas: F(2k) = F(k)(2F(k+1)-F(k)),
 *                              F(2k+1) = F(k)^2 + F(k+1)^2
 */

const long long MOD = 1e9 + 7;

typedef vector<vector<long long>> Matrix;

/* 2x2 matrix multiplication mod p */
Matrix mat_mul(const Matrix& A, const Matrix& B) {
    Matrix C(2, vector<long long>(2, 0));
    for (int i = 0; i < 2; i++)
        for (int k = 0; k < 2; k++)
            for (int j = 0; j < 2; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
    return C;
}

/* Matrix binary exponentiation */
Matrix mat_pow(Matrix M, long long n) {
    Matrix R = {{1, 0}, {0, 1}};  // identity
    while (n > 0) {
        if (n & 1) R = mat_mul(R, M);
        M = mat_mul(M, M);
        n >>= 1;
    }
    return R;
}

/* Method 1: F(n) via matrix exponentiation */
long long fib_matrix(long long n) {
    if (n <= 0) return 0;
    if (n <= 2) return 1;
    Matrix Q = {{1, 1}, {1, 0}};
    Matrix R = mat_pow(Q, n - 1);
    return R[0][0];
}

/* Method 2: F(n) via fast doubling - returns (F(n), F(n+1)) */
pair<long long, long long> fib_doubling(long long n) {
    if (n == 0) return {0, 1};
    auto [a, b] = fib_doubling(n >> 1);
    long long c = a % MOD * ((2 * b - a + MOD) % MOD) % MOD;
    long long d = (a % MOD * a % MOD + b % MOD * b % MOD) % MOD;
    if (n & 1)
        return {d, (c + d) % MOD};
    else
        return {c, d};
}

long long fib_fast(long long n) {
    return fib_doubling(n).first;
}

int main() {
    long long N = 10000000;  // 10^7

    // Method 1: matrix
    long long f_mat = fib_matrix(N + 2);
    long long ans1 = (f_mat - 1 + MOD) % MOD;

    // Method 2: fast doubling
    long long f_dbl = fib_fast(N + 2);
    long long ans2 = (f_dbl - 1 + MOD) % MOD;

    assert(ans1 == ans2);

    // Verify small cases
    assert(fib_fast(1) == 1);
    assert(fib_fast(2) == 1);
    assert(fib_fast(10) == 55);
    assert(fib_fast(20) == 6765);

    // Verify telescoping for n=10
    long long sum10 = 0, a = 1, b = 1;
    for (int k = 1; k <= 10; k++) {
        sum10 += a;
        long long t = a;
        a = b;
        b = (t + b);
    }
    assert(sum10 == 143);  // F(12) - 1 = 144 - 1 = 143

    cout << ans1 << endl;
    return 0;
}
