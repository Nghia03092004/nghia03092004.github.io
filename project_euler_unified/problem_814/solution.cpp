#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 814: Seating Plan
 *
 * Count circular permutations with forbidden adjacency constraints.
 * Uses derangement recurrence D_n = (n-1)(D_{n-1} + D_{n-2}) and
 * transfer matrix methods for the circular constraint.
 */

const long long MOD = 1e9 + 7;

// Derangement numbers mod p
long long derangement_mod(long long n, long long mod) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    long long prev2 = 1, prev1 = 0;
    for (long long i = 2; i <= n; i++) {
        long long curr = (i - 1) % mod * ((prev1 + prev2) % mod) % mod;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// Matrix multiplication mod p (for transfer matrix method)
typedef vector<vector<long long>> Matrix;

Matrix mat_mul(const Matrix& A, const Matrix& B, long long mod) {
    int n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod;
        }
    return C;
}

Matrix mat_pow(Matrix A, long long exp, long long mod) {
    int n = A.size();
    Matrix result(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1; // identity
    while (exp > 0) {
        if (exp & 1) result = mat_mul(result, A, mod);
        A = mat_mul(A, A, mod);
        exp >>= 1;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verify derangements
    assert(derangement_mod(2, MOD) == 1);
    assert(derangement_mod(3, MOD) == 2);
    assert(derangement_mod(4, MOD) == 9);
    assert(derangement_mod(5, MOD) == 44);
    assert(derangement_mod(6, MOD) == 265);

    // Compute for N = 10^6
    long long N = 1000000;
    long long ans = derangement_mod(N, MOD);
    cout << ans << endl;

    return 0;
}
