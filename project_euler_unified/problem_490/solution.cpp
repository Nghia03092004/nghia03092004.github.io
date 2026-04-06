#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<vector<ll>> Matrix;

const ll MOD = 1e9 + 7;

Matrix mat_mult(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) if (A[i][k])
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
    return C;
}

Matrix mat_pow(Matrix M, ll p) {
    int n = M.size();
    Matrix result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1; // identity
    while (p > 0) {
        if (p & 1) result = mat_mult(result, M);
        M = mat_mult(M, M);
        p >>= 1;
    }
    return result;
}

ll solve(ll n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;

    Matrix A = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}};
    Matrix An = mat_pow(A, n - 2);
    // f(n) = An[0][0]*f(2) + An[0][1]*f(1) + An[0][2]*f(0)
    return (An[0][0] * 2 + An[0][1] + An[0][2]) % MOD;
}

int main() {
    // Print first few values
    cout << "Frog jumping paths f(n):" << endl;
    for (int n = 0; n <= 20; n++) {
        cout << "f(" << n << ") = " << solve(n) << endl;
    }

    // Large value
    ll big = 1000000000000000000LL;
    cout << "\nf(10^18) mod 10^9+7 = " << solve(big) << endl;

    return 0;
}
