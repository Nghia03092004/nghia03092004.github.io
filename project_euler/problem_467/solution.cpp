/*
 * Project Euler Problem 467: Superinteger
 *
 * Find f(10000) mod 1,000,000,007 where f(n) is the smallest superinteger
 * (shortest common supersequence) of the digital root sequences of the
 * first n primes and first n composites.
 *
 * Algorithm:
 *   1. Generate digital root sequences P_n and C_n
 *   2. Compute suffix LCS table: lcs[i][j] = LCS(A[i:], B[j:])
 *   3. Build lexicographically smallest SCS forward using greedy:
 *      At state (i,j), next char must be A[i] or B[j] (or both if equal).
 *      Pick the option that keeps SCS length optimal, break ties by
 *      choosing the smaller digit.
 *   4. Compute numeric value mod 10^9+7 during construction.
 *
 * Compile: g++ -O2 -o solution solution.cpp
 * Memory: ~400MB for n=10000 (short array 10001x10001)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const long long MOD = 1000000007LL;
const int N = 10000;
const int SIEVE_LIMIT = 200000;

bool is_prime_arr[SIEVE_LIMIT + 1];

int digital_root(int n) {
    if (n == 0) return 0;
    return 1 + (n - 1) % 9;
}

void sieve() {
    memset(is_prime_arr, true, sizeof(is_prime_arr));
    is_prime_arr[0] = is_prime_arr[1] = false;
    for (int i = 2; i * i <= SIEVE_LIMIT; i++) {
        if (is_prime_arr[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime_arr[j] = false;
            }
        }
    }
}

// Suffix LCS table: lcs[i][j] = LCS(A[i:], B[j:])
short lcs_table[N + 1][N + 1];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    sieve();

    // Generate sequences
    vector<int> A, B; // A = sp (primes), B = sc (composites)
    for (int i = 2; i <= SIEVE_LIMIT && ((int)A.size() < N || (int)B.size() < N); i++) {
        if (is_prime_arr[i]) {
            if ((int)A.size() < N) A.push_back(digital_root(i));
        } else {
            if ((int)B.size() < N) B.push_back(digital_root(i));
        }
    }

    int m = A.size(), n = B.size();
    cout << "Generated sequences: m=" << m << ", n=" << n << endl;
    cout << "P starts: ";
    for (int i = 0; i < 20; i++) cout << A[i];
    cout << endl;
    cout << "C starts: ";
    for (int i = 0; i < 20; i++) cout << B[i];
    cout << endl;

    // Compute suffix LCS
    cout << "Computing suffix LCS table..." << endl;
    for (int i = 0; i <= m; i++) lcs_table[i][n] = 0;
    for (int j = 0; j <= n; j++) lcs_table[m][j] = 0;

    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            if (A[i] == B[j]) {
                lcs_table[i][j] = lcs_table[i+1][j+1] + 1;
            } else {
                lcs_table[i][j] = max(lcs_table[i+1][j], lcs_table[i][j+1]);
            }
        }
    }

    int lcs_len = lcs_table[0][0];
    int scs_len = m + n - lcs_len;
    cout << "LCS length: " << lcs_len << endl;
    cout << "SCS length: " << scs_len << endl;

    // Build lexicographically smallest SCS forward
    cout << "Building SCS..." << endl;
    long long val = 0;
    int ci = 0, cj = 0;

    auto scs_rem = [&](int i, int j) -> int {
        return (m - i) + (n - j) - lcs_table[i][j];
    };

    while (ci < m || cj < n) {
        int d;
        if (ci == m) {
            d = B[cj]; cj++;
        } else if (cj == n) {
            d = A[ci]; ci++;
        } else if (A[ci] == B[cj]) {
            d = A[ci]; ci++; cj++;
        } else {
            int cur = scs_rem(ci, cj);
            bool opt_a = (1 + scs_rem(ci + 1, cj) == cur);
            bool opt_b = (1 + scs_rem(ci, cj + 1) == cur);

            if (opt_a && opt_b) {
                if (A[ci] <= B[cj]) {
                    d = A[ci]; ci++;
                } else {
                    d = B[cj]; cj++;
                }
            } else if (opt_a) {
                d = A[ci]; ci++;
            } else {
                d = B[cj]; cj++;
            }
        }
        val = (val * 10 + d) % MOD;
    }

    cout << endl;
    cout << "f(" << N << ") mod " << MOD << " = " << val << endl;

    // Verify small case n=10
    {
        cout << "\n=== Verification n=10 ===" << endl;
        vector<int> a10(A.begin(), A.begin() + 10);
        vector<int> b10(B.begin(), B.begin() + 10);

        short lcs10[11][11];
        memset(lcs10, 0, sizeof(lcs10));
        for (int i = 9; i >= 0; i--)
            for (int j = 9; j >= 0; j--) {
                if (a10[i] == b10[j])
                    lcs10[i][j] = lcs10[i+1][j+1] + 1;
                else
                    lcs10[i][j] = max(lcs10[i+1][j], lcs10[i][j+1]);
            }

        cout << "LCS(10) = " << lcs10[0][0] << endl;
        cout << "SCS(10) = " << 20 - lcs10[0][0] << endl;

        auto scs_rem10 = [&](int i, int j) -> int {
            return (10 - i) + (10 - j) - lcs10[i][j];
        };

        string scs_str;
        int ii = 0, jj = 0;
        while (ii < 10 || jj < 10) {
            int dd;
            if (ii == 10) { dd = b10[jj]; jj++; }
            else if (jj == 10) { dd = a10[ii]; ii++; }
            else if (a10[ii] == b10[jj]) { dd = a10[ii]; ii++; jj++; }
            else {
                int cur = scs_rem10(ii, jj);
                bool oa = (1 + scs_rem10(ii+1, jj) == cur);
                bool ob = (1 + scs_rem10(ii, jj+1) == cur);
                if (oa && ob) {
                    if (a10[ii] <= b10[jj]) { dd = a10[ii]; ii++; }
                    else { dd = b10[jj]; jj++; }
                } else if (oa) { dd = a10[ii]; ii++; }
                else { dd = b10[jj]; jj++; }
            }
            scs_str += ('0' + dd);
        }
        cout << "f(10) = " << scs_str << endl;
        cout << "Expected: 2357246891352679" << endl;

        long long v = 0;
        for (char c : scs_str) v = (v * 10 + (c - '0')) % MOD;
        cout << "f(10) mod " << MOD << " = " << v << endl;
    }

    return 0;
}
