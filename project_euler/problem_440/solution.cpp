#include <bits/stdc++.h>
using namespace std;

// Project Euler 440: GCD and Tiling
// T(n) = 10*T(n-1) + T(n-2), T(0)=1, T(1)=10
// gcd(T(m), T(n)) = T(gcd(m,n))
// S(L) = sum_{c=1}^L sum_{k=1}^L T(c^k) * (2L - 2k + 1)
// Find S(2000) mod 987898789

typedef long long ll;
typedef vector<vector<ll>> Matrix;

const ll MOD = 987898789LL;
const int L = 2000;

Matrix mul(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
        }
    return C;
}

Matrix mat_pow(Matrix A, ll p) {
    int n = A.size();
    Matrix result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1; // identity
    while (p > 0) {
        if (p & 1) result = mul(result, A);
        A = mul(A, A);
        p >>= 1;
    }
    return result;
}

// Extract T(n) from A^n * [10, 1]^T: T(n) = A^n[1][0]*10 + A^n[1][1]
// Actually: [T(n+1), T(n)] = A^n * [T(1), T(0)] = A^n * [10, 1]
// So T(n) = A^n[1][0]*10 + A^n[1][1]
ll get_T(const Matrix& An) {
    return (An[1][0] * 10 + An[1][1]) % MOD;
}

int main() {
    // Base matrix A = [[10, 1], [1, 0]]
    Matrix A = {{10, 1}, {1, 0}};

    ll ans = 0;

    for (int c = 1; c <= L; c++) {
        // Compute A^c
        Matrix Mc = mat_pow(A, c);

        // For k=1: T(c^1) = T(c)
        ll Tc = get_T(Mc);
        ll weight = (2LL * L - 2 * 1 + 1) % MOD;
        ans = (ans + Tc * weight) % MOD;

        // For k=2 to L: M_{k} = M_{k-1}^c, giving A^(c^k)
        for (int k = 2; k <= L; k++) {
            Mc = mat_pow(Mc, c);
            Tc = get_T(Mc);
            weight = (2LL * L - 2 * k + 1);
            if (weight <= 0) weight = (weight % MOD + MOD) % MOD;
            ans = (ans + Tc % MOD * (weight % MOD)) % MOD;
        }

        if (c % 100 == 0)
            fprintf(stderr, "c = %d / %d done\n", c, L);
    }

    printf("%lld\n", ans);
    return 0;
}
