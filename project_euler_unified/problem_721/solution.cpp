#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 721: High Powers of Irrational Numbers
 *
 * For non-perfect-square a, f(a,n) = floor((ceil(sqrt(a)) + sqrt(a))^n).
 * Using the companion algebraic integer beta = ceil(sqrt(a)) - sqrt(a),
 * we have alpha^n + beta^n = T_n (integer), and f(a,n) = T_n - 1.
 *
 * T_n satisfies: T_k = 2m * T_{k-1} - (m^2-a) * T_{k-2}
 * where m = ceil(sqrt(a)).
 *
 * We compute T_{a^2} mod p via 2x2 matrix exponentiation.
 */

const long long MOD = 999999937LL;

typedef vector<vector<long long>> Mat;

Mat mat_mul(const Mat& A, const Mat& B, long long mod) {
    int n = A.size();
    Mat C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) if (A[i][k])
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
    return C;
}

Mat mat_pow(Mat M, long long p, long long mod) {
    int n = M.size();
    Mat result(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1;
    while (p > 0) {
        if (p & 1) result = mat_mul(result, M, mod);
        M = mat_mul(M, M, mod);
        p >>= 1;
    }
    return result;
}

long long compute_T(long long m, long long c, long long n, long long mod) {
    // T_k = 2m * T_{k-1} - c * T_{k-2}, T_0=2, T_1=2m
    if (n == 0) return 2 % mod;
    if (n == 1) return (2 * m) % mod;
    Mat M = {{(2*m) % mod, (mod - c % mod) % mod}, {1, 0}};
    Mat R = mat_pow(M, n - 1, mod);
    return (R[0][0] * (2*m % mod) % mod + R[0][1] * 2 % mod) % mod;
}

bool is_perfect_square(long long a) {
    long long s = (long long)sqrt((double)a);
    while (s * s > a) s--;
    while ((s+1)*(s+1) <= a) s++;
    return s * s == a;
}

int main() {
    long long N = 1000; // Change to 5000000 for full answer
    long long total = 0;

    for (long long a = 1; a <= N; a++) {
        if (is_perfect_square(a)) continue;
        long long s = (long long)sqrt((double)a);
        while (s * s > a) s--;
        long long m = s + 1;
        long long c = m * m - a;
        long long n = a * a;
        long long T = compute_T(m, c, n, MOD);
        total = (total + T - 1 + MOD) % MOD;
    }

    cout << total << endl;
    return 0;
}
