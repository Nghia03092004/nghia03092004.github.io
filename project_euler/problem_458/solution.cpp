/*
 * Project Euler Problem 458: Permutations of Project
 *
 * T(n) = number of strings of length n over {p,r,o,j,e,c,t} that do NOT
 * contain any permutation of "project" as a contiguous substring.
 *
 * T(7) = 7^7 - 7! = 818503.
 * Find T(10^12) mod 10^9.
 *
 * Approach: 8-state transfer matrix + matrix exponentiation.
 *
 * Compile: g++ -O2 -o solution solution.cpp
 */

#include <cstdio>
#include <cstring>
#include <cstdint>

typedef long long ll;
typedef unsigned long long ull;

const ll MOD = 1000000000LL;  // 10^9
const int SZ = 8;

struct Matrix {
    ll m[SZ][SZ];

    Matrix() { memset(m, 0, sizeof(m)); }

    static Matrix identity() {
        Matrix I;
        for (int i = 0; i < SZ; i++) I.m[i][i] = 1;
        return I;
    }

    Matrix operator*(const Matrix& B) const {
        Matrix C;
        for (int i = 0; i < SZ; i++)
            for (int k = 0; k < SZ; k++) {
                if (m[i][k] == 0) continue;
                for (int j = 0; j < SZ; j++)
                    C.m[i][j] = (C.m[i][j] + m[i][k] * B.m[k][j]) % MOD;
            }
        return C;
    }
};

Matrix mat_pow(Matrix base, ll exp) {
    Matrix result = Matrix::identity();
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/*
 * Build transition matrix M[to][from]:
 *
 * From state s (0 <= s <= 6):
 *   - To state s+1: (7-s) ways (new distinct char)
 *   - To state i for i=1..s: 1 way (duplicate of i-th from right)
 * From state 0:
 *   - To state 1: 7 ways
 * From state 7 (absorbing):
 *   - To state 7: 7 ways
 */
Matrix build_transition() {
    Matrix M;

    for (int s = 0; s < 7; s++) {
        M.m[s + 1][s] = 7 - s;  // new char
        for (int i = 1; i <= s; i++) {
            M.m[i][s] += 1;  // duplicate at position i
        }
    }
    M.m[7][7] = 7;  // absorbing

    return M;
}

int main() {
    ll n = 1000000000000LL;  // 10^12

    // Verify T(7)
    {
        Matrix M = build_transition();
        Matrix M7 = mat_pow(M, 7);
        ll t7 = 0;
        for (int s = 0; s < 7; s++)
            t7 = (t7 + M7.m[s][0]) % MOD;
        printf("T(7) mod 10^9 = %lld (expected 818503)\n", t7);
    }

    // Compute T(10^12) mod 10^9
    Matrix M = build_transition();
    Matrix Mn = mat_pow(M, n);

    ll ans = 0;
    for (int s = 0; s < 7; s++)
        ans = (ans + Mn.m[s][0]) % MOD;

    printf("T(%lld) mod %lld = %lld\n", n, MOD, ans);

    return 0;
}
