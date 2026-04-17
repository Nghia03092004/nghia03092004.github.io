#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<vector<ll>> Matrix;

const ll MOD = 1000000000LL;
const int S_MAX = 24; // digit sums 0..23
const int R_MAX = 23; // remainders mod 23: 0..22
const int DIM = S_MAX * R_MAX; // 552

// State encoding: (s, r) -> s * R_MAX + r
int encode(int s, int r) { return s * R_MAX + r; }

Matrix mat_mul(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < n; j++) {
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
            }
        }
    return C;
}

Matrix mat_pow(Matrix M, ll p) {
    int n = M.size();
    Matrix R(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) R[i][i] = 1; // identity
    while (p > 0) {
        if (p & 1) R = mat_mul(R, M);
        M = mat_mul(M, M);
        p >>= 1;
    }
    return R;
}

int main() {
    // Build transition matrix
    Matrix T(DIM, vector<ll>(DIM, 0));
    for (int s = 0; s < S_MAX; s++) {
        for (int r = 0; r < R_MAX; r++) {
            for (int d = 0; d <= 9; d++) {
                int ns = s + d;
                if (ns >= S_MAX) continue;
                int nr = (10 * r + d) % R_MAX;
                int from = encode(s, r);
                int to = encode(ns, nr);
                T[to][from] = (T[to][from] + 1) % MOD;
            }
        }
    }

    // Compute T^n where n = 11^12
    // 11^12 = 3138428376721
    ll n = 1;
    for (int i = 0; i < 12; i++) n *= 11;
    // n = 3138428376721

    Matrix Tn = mat_pow(T, n);

    // Answer: Tn[encode(23, 0)][encode(0, 0)]
    ll ans = Tn[encode(23, 0)][encode(0, 0)];
    cout << ans << endl;

    return 0;
}
