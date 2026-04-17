#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 903: Matrix Permanent Modulo a Prime
 *
 * A[i][j] = (i*j) mod n for i,j in {1,...,n}, n = 12.
 * Find perm(A) mod 10^9+7.
 *
 * Key insight: Row n has A[n][j] = (n*j) mod n = 0 for all j.
 * A matrix with a zero row has permanent = 0.
 *
 * Two methods:
 *   1. Zero-row detection (O(n) check)
 *   2. Ryser's formula (O(2^n * n) general computation)
 */

const long long MOD = 1e9 + 7;

/*
 * Method 1: Check for zero row
 * If any row i has A[i][j] = 0 for all j, perm = 0.
 * For the multiplication table mod n, row n always satisfies this.
 */
bool has_zero_row(int n) {
    for (int i = 1; i <= n; i++) {
        bool all_zero = true;
        for (int j = 1; j <= n; j++) {
            if ((i * j) % n != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) return true;
    }
    return false;
}

/*
 * Method 2: Ryser's formula
 * perm(A) = (-1)^n * sum_{S} (-1)^|S| * prod_i (sum_{j in S} A[i][j])
 *
 * O(2^n * n) time.  For n=12, 2^12 * 12 = 49152 operations.
 */
long long permanent_ryser(vector<vector<int>>& A) {
    int n = A.size();
    long long result = 0;

    for (int mask = 1; mask < (1 << n); mask++) {
        int bits = __builtin_popcount(mask);

        // Product of row sums for columns in mask
        long long prod = 1;
        bool zero = false;
        for (int i = 0; i < n && !zero; i++) {
            long long row_sum = 0;
            for (int j = 0; j < n; j++) {
                if (mask & (1 << j))
                    row_sum += A[i][j];
            }
            if (row_sum == 0) { zero = true; break; }
            prod *= row_sum;
        }
        if (zero) continue;

        long long sign = ((n - bits) % 2 == 0) ? 1 : -1;
        result += sign * prod;
    }

    // Multiply by (-1)^n
    if (n % 2 == 1) result = -result;
    return result;
}

int main() {
    int n = 12;

    // Method 1: structural
    if (has_zero_row(n)) {
        cout << 0 << endl;
    }

    // Method 2: Ryser verification
    vector<vector<int>> A(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = ((i + 1) * (j + 1)) % n;

    long long perm = permanent_ryser(A);
    assert(perm == 0);

    // Verify for small n: all should be 0
    for (int nn = 2; nn <= 8; nn++) {
        vector<vector<int>> M(nn, vector<int>(nn));
        for (int i = 0; i < nn; i++)
            for (int j = 0; j < nn; j++)
                M[i][j] = ((i + 1) * (j + 1)) % nn;
        assert(permanent_ryser(M) == 0);
    }

    cout << 0 << endl;
    return 0;
}
