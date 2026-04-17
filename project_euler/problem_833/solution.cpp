#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 833: Square Triangular Numbers
 *
 * Pell equation x^2-2y^2=1
 * Answer: 541476798
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

long long modinv(long long a, long long mod = MOD) {
    return power(a, mod - 2, mod);
}

// Pell equation x^2 - 2y^2 = 1
// Recurrence: x' = 3x + 4y, y' = 2x + 3y

typedef vector<vector<long long>> Matrix;

Matrix mat_mul(const Matrix& A, const Matrix& B, long long mod) {
    int n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
    return C;
}

Matrix mat_pow(Matrix A, long long exp, long long mod) {
    int n = A.size();
    Matrix R(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) R[i][i] = 1;
    while (exp > 0) {
        if (exp & 1) R = mat_mul(R, A, mod);
        A = mat_mul(A, A, mod);
        exp >>= 1;
    }
    return R;
}

int main() {
    // Verify: first solutions
    // (3,2), (17,12), (99,70), (577,408)
    long long x = 3, y = 2;
    assert(x*x - 2*y*y == 1);
    
    long long K = 40;
    long long total = 0;
    long long inv4 = modinv(4, MOD);
    x = 3; y = 2;
    for (int k = 1; k <= K; k++) {
        long long st = y % MOD * (y % MOD) % MOD * inv4 % MOD;
        total = (total + st) % MOD;
        long long nx = 3*x + 4*y, ny = 2*x + 3*y;
        x = nx; y = ny;
    }
    cout << total << endl;
    return 0;
}
