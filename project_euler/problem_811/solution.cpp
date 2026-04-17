#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 811: Bitwise Recursion
 *
 * f(n, k) = n                    if k = 0
 *         = f(n XOR k, k >> 1)   if k > 0
 *
 * Key insight: f(n, k) = n XOR g(k), where g(k) = k XOR (k>>1) XOR (k>>2) XOR ...
 * Bit j of g(k) = XOR of bits j, j+1, j+2, ... of k (suffix XOR parity).
 *
 * S(N) = sum_{k=0}^{N} sum_{n=0}^{N} (n XOR g(k))
 *
 * For each bit position j, count pairs (n,k) in [0,N]^2 where
 * bit j of (n XOR g(k)) is 1, then S(N) = sum_j 2^j * count_j.
 */

const long long MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Count k in [0..N] where suffix XOR parity from bit j onward is 1
// Uses digit DP on binary representation of N
long long count_suffix_xor_odd(long long N, int j) {
    if (N < 0) return 0;
    int B = 63 - __builtin_clzll(N + 1);  // number of bits needed
    if (j > B) return 0;

    // Extract bits of N from MSB to LSB
    vector<int> bits(B + 1);
    for (int i = B; i >= 0; i--) {
        bits[B - i] = (N >> i) & 1;
    }

    // dp[tight][parity] = count
    // tight: are we still bounded by N?
    // parity: XOR of bits at positions >= j seen so far
    map<pair<bool,int>, long long> dp;
    dp[{true, 0}] = 1;

    for (int i = 0; i <= B; i++) {
        int actual_bit_pos = B - i;
        map<pair<bool,int>, long long> new_dp;
        for (auto& [state, cnt] : dp) {
            auto [tight, par] = state;
            int max_d = tight ? bits[i] : 1;
            for (int d = 0; d <= max_d; d++) {
                bool new_tight = tight && (d == bits[i]);
                int new_par = par;
                if (actual_bit_pos >= j) {
                    new_par = par ^ d;
                }
                new_dp[{new_tight, new_par}] += cnt;
            }
        }
        dp = new_dp;
    }

    long long result = 0;
    for (auto& [state, cnt] : dp) {
        if (state.second == 1) result += cnt;
    }
    return result;
}

// Count n in [0..N] with bit j set
long long count_bit_set(long long N, int j) {
    if (N < 0) return 0;
    long long full = (N + 1) >> (j + 1);
    long long rem = (N + 1) & ((1LL << (j + 1)) - 1);
    return full * (1LL << j) + max(0LL, rem - (1LL << j));
}

long long solve(long long N) {
    int B = 63;
    if (N > 0) B = 63 - __builtin_clzll(N);

    long long total = 0;
    for (int j = 0; j <= B; j++) {
        long long ones_n = count_bit_set(N, j) % MOD;
        long long zeros_n = ((N + 1) % MOD - ones_n % MOD + MOD) % MOD;

        long long ones_k = count_suffix_xor_odd(N, j) % MOD;
        long long zeros_k = ((N + 1) % MOD - ones_k % MOD + MOD) % MOD;

        // Pairs where n_j XOR g(k)_j = 1
        long long count_j = (ones_n * zeros_k % MOD + zeros_n * ones_k % MOD) % MOD;
        total = (total + count_j % MOD * power(2, j, MOD) % MOD) % MOD;
    }
    return total;
}

// Brute force for verification
long long g_func(long long k) {
    long long result = 0;
    while (k) {
        result ^= k;
        k >>= 1;
    }
    return result;
}

long long solve_brute(int N) {
    long long total = 0;
    for (int k = 0; k <= N; k++) {
        long long gk = g_func(k);
        for (int n = 0; n <= N; n++) {
            total += (n ^ gk);
        }
    }
    return total;
}

int main() {
    // Cross-verify on small cases
    for (int N = 1; N <= 20; N++) {
        long long brute = solve_brute(N);
        long long dp = solve(N);
        assert(brute % MOD == dp);
    }

    // Compute answer for N = 10^18
    long long N = 1000000000000000000LL;
    cout << solve(N) << endl;

    return 0;
}
