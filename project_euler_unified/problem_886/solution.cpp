#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 886: Coprime Permutations
 *
 * Count permutations sigma of {1..n} with gcd(i, sigma(i)) = 1 for all i.
 * This equals the permanent of the coprimality matrix, computed via Ryser's formula.
 *
 * For n=34, full Ryser's formula with 2^34 subsets is too slow in pure form.
 * We use Glynn's formula variant which also runs in O(2^n * n) but with better constants,
 * and split the computation using a meet-in-the-middle approach.
 *
 * Actually for n=34 with modular arithmetic, we can use the standard inclusion-exclusion
 * over columns with Gray code. Each step is O(n) work = 34 multiplies.
 * 2^34 * 34 ~ 5.8 * 10^11 operations - too slow for 60 seconds.
 *
 * Better approach: Split columns into two halves of size 17 each.
 * For each row i, precompute the generating function over columns in each half.
 * Then combine using convolution. This gives O(2^(n/2) * n^2) ~ 2^17 * 34^2 ~ 1.5*10^8.
 *
 * Meet in the middle for permanent:
 * perm(A) = sum over all sigma: prod A[i][sigma(i)]
 *
 * Alternatively, we use the formula:
 * perm(A) = (-1)^n * sum_{S subset [n]} (-1)^|S| * prod_{i=1}^{n} (sum_{j in S} A[i][j])
 *
 * Split columns into L = {1..17} and R = {18..34}.
 * For subset S, let S_L = S intersect L, S_R = S intersect R.
 * For row i, sum_{j in S} A[i][j] = f_i(S_L) + g_i(S_R)
 * where f_i(S_L) = sum_{j in S_L} A[i][j], g_i(S_R) = sum_{j in S_R} A[i][j].
 *
 * prod_{i=1}^{n} (f_i(S_L) + g_i(S_R))
 *
 * This doesn't factor nicely for meet-in-the-middle since the product couples all rows.
 *
 * For n=34, let's just do the direct Ryser computation with optimizations.
 * With careful C++ and compiler optimizations, 2^34 iterations with ~34 operations each
 * should run in about 30-60 seconds.
 */

const long long MOD = 83456729;

int main(){
    ios::sync_with_stdio(false);
    const int n = 34;

    // Build coprimality matrix
    // A[i][j] = 1 if gcd(i+1, j+1) == 1 (0-indexed)
    int A[n][n];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            A[i][j] = (__gcd(i+1, j+1) == 1) ? 1 : 0;

    // Ryser's formula with Gray code enumeration
    // perm(A) = (-1)^n * sum_{S} (-1)^|S| * prod_i (row_sum_i(S))
    // where row_sum_i(S) = sum_{j in S} A[i][j]

    long long row_sum[n];
    memset(row_sum, 0, sizeof(row_sum));

    long long result = 0;
    long long sign = 1; // (-1)^|S|, starts with empty set |S|=0
    // Empty set contribution: prod_i(0) = 0 (for n>=1), so skip

    // Total subsets: 2^n = 2^34
    long long total = 1LL << n;

    for(long long gray_idx = 1; gray_idx < total; gray_idx++){
        // Find which bit changed (Gray code)
        long long gray_prev = (gray_idx - 1) ^ ((gray_idx - 1) >> 1);
        long long gray_curr = gray_idx ^ (gray_idx >> 1);
        long long diff = gray_prev ^ gray_curr;
        int j = __builtin_ctzll(diff); // column that changed

        int adding = (gray_curr >> j) & 1; // 1 if adding column j, 0 if removing

        if(adding){
            for(int i = 0; i < n; i++)
                row_sum[i] += A[i][j];
            sign = (MOD - sign) % MOD; // flip sign
        } else {
            for(int i = 0; i < n; i++)
                row_sum[i] -= A[i][j];
            sign = (MOD - sign) % MOD;
        }

        // Compute product of row_sums mod MOD
        long long prod = sign;
        bool zero = false;
        for(int i = 0; i < n; i++){
            long long rs = row_sum[i] % MOD;
            if(rs < 0) rs += MOD;
            if(rs == 0){ zero = true; break; }
            prod = prod * rs % MOD;
        }
        if(!zero){
            result = (result + prod) % MOD;
        }
    }

    // Multiply by (-1)^n
    if(n % 2 == 1){
        result = (MOD - result) % MOD;
    }

    cout << result << endl;
    return 0;
}
