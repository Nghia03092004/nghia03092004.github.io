#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 237: Tours on a 4 x n Playing Board
 *
 * T(n) = number of Hamiltonian paths on 4 x n grid from (0,0) to (3,0).
 * Find T(10^12) mod 10^8.
 *
 * Recurrence (found by brute-force + Gaussian elimination):
 *   T(n) = 2*T(n-1) + 2*T(n-2) - 2*T(n-3) + T(n-4)
 *   T(1)=1, T(2)=1, T(3)=4, T(4)=8
 *
 * Verified: T(10) = 2329.
 *
 * Use 4x4 companion matrix exponentiation modulo 10^8.
 */

typedef long long ll;
typedef vector<vector<ll>> Mat;

const ll MOD = 1e8;

Mat mat_mul(const Mat& A, const Mat& B) {
    int n = A.size();
    Mat C(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
        }
    return C;
}

Mat mat_pow(Mat M, ll p) {
    int n = M.size();
    Mat R(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) R[i][i] = 1;
    while (p > 0) {
        if (p & 1) R = mat_mul(R, M);
        M = mat_mul(M, M);
        p >>= 1;
    }
    return R;
}

int main(){
    // Companion matrix for T(n) = 2T(n-1) + 2T(n-2) - 2T(n-3) + T(n-4)
    Mat M = {
        {2, 2, MOD - 2, 1},
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0}
    };

    ll N = 1000000000000LL; // 10^12

    // State at n=4: [T(4), T(3), T(2), T(1)] = [8, 4, 1, 1]
    Mat Mk = mat_pow(M, N - 4);

    vector<ll> state = {8, 4, 1, 1};
    ll ans = 0;
    for (int j = 0; j < 4; j++)
        ans = (ans + Mk[0][j] * state[j]) % MOD;

    cout << ans << endl;

    return 0;
}
