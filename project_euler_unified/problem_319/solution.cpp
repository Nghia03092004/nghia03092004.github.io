#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 319: Bounded Sequences
 *
 * t(n) = sum_{k=1}^{n} mu(k) * g(floor(n/k))
 * where g(m) = G(3,m) - G(2,m), G(v,m) = v*(v^m - 1)/(v-1)
 *
 * Uses:
 * - Sieve for Mobius function prefix sums up to LMT
 * - Mertens function via recursive formula with memoization for large values
 * - Matrix exponentiation for G(v,m) mod MOD
 *
 * Answer: 268457129
 */

typedef long long i64;

const i64 N = 10000000000LL; // 10^10
const i64 MOD = 1000000000LL; // 10^9
const int LMT = 10000000;     // 10^7

i64 mu_prefix[LMT + 1]; // prefix sum of Mobius function
map<i64, i64> memo_mertens;

// Compute Mertens function M(n) = sum_{k=1}^{n} mu(k), modulo MOD
i64 mertens(i64 n) {
    if (n <= LMT) return mu_prefix[n];
    if (memo_mertens.count(n)) return memo_mertens[n];
    i64 ret = 1; // M(1) = mu(1) = 1
    for (i64 i = 2, j; i <= n; i = j) {
        j = n / (n / i) + 1;
        ret = (ret + MOD - (j - i) % MOD * mertens(n / i) % MOD) % MOD;
    }
    return memo_mertens[n] = ret;
}

// 2x2 matrix multiplication mod MOD
typedef i64 mat[2][2];

void matmul(mat a, mat b, mat c) {
    mat t;
    memset(t, 0, sizeof(mat));
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                t[i][k] = (t[i][k] + a[i][j] % MOD * (b[j][k] % MOD)) % MOD;
    memcpy(c, t, sizeof(mat));
}

// G(v, x) = v * (v^x - 1) / (v - 1) mod MOD, via matrix exponentiation
// [v 1; 0 1]^x has entry [0][1] = (v^x - 1)/(v - 1)
i64 G(int v, i64 x) {
    if (x == 0) return 0;
    mat A, B;
    A[0][0] = v; A[0][1] = 1;
    A[1][0] = 0; A[1][1] = 1;
    memset(B, 0, sizeof(B));
    B[0][0] = B[1][1] = 1; // identity
    for (i64 e = x; e; e >>= 1) {
        if (e & 1) matmul(B, A, B);
        matmul(A, A, A);
    }
    return (i64)v % MOD * (B[0][1] % MOD) % MOD;
}

i64 g(i64 x) {
    return (G(3, x) - G(2, x) + MOD) % MOD;
}

int main() {
    // Sieve Mobius function and compute prefix sums
    vector<int> mu_val(LMT + 1, 0);
    vector<bool> is_composite(LMT + 1, false);
    vector<int> primes;
    mu_val[1] = 1;

    for (int i = 2; i <= LMT; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            mu_val[i] = -1;
        }
        for (int j = 0; j < (int)primes.size() && (i64)i * primes[j] <= LMT; j++) {
            int t = i * primes[j];
            is_composite[t] = true;
            if (i % primes[j] == 0) {
                mu_val[t] = 0;
                break;
            } else {
                mu_val[t] = -mu_val[i];
            }
        }
    }

    mu_prefix[0] = 0;
    for (int i = 1; i <= LMT; i++)
        mu_prefix[i] = (mu_prefix[i - 1] + mu_val[i] % MOD + MOD) % MOD;

    // Compute t(N) = sum_{k=1}^{N} mu(k) * g(floor(N/k))
    // Group by distinct values of floor(N/k)
    i64 ans = 0;
    i64 prev_mertens = 0; // M(0) = 0
    for (i64 i = 1, j; i <= N; i = j) {
        j = N / (N / i) + 1;
        i64 cur_mertens = mertens(j - 1);
        i64 block_mu_sum = (cur_mertens - prev_mertens + MOD) % MOD;
        ans = (ans + block_mu_sum % MOD * g(N / i) % MOD) % MOD;
        prev_mertens = cur_mertens;
    }

    cout << ans % MOD << endl;
    return 0;
}
