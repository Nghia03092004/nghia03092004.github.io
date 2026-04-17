#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 840: Sum of Products
 *
 * Compute sum_{k=1}^{n} e_k(x_1,...,x_n) mod p.
 * Key: sum = prod(1 + x_i) - 1.
 *
 * Also implements Newton's identities for verification.
 */

const long long MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
    long long result = 1; base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod; exp >>= 1;
    }
    return result;
}

// Method 1: Product formula
long long solve_product(const vector<long long>& xs) {
    long long result = 1;
    for (long long x : xs)
        result = result % MOD * ((1 + x) % MOD) % MOD;
    return (result - 1 + MOD) % MOD;
}

// Method 2: Newton's identities (for small n verification)
long long solve_newton(const vector<long long>& xs) {
    int n = xs.size();
    vector<long long> p(n + 1, 0), e(n + 1, 0);
    e[0] = 1;
    for (int j = 1; j <= n; j++)
        for (long long x : xs)
            p[j] = (p[j] + power(x, j, MOD)) % MOD;
    for (int k = 1; k <= n; k++) {
        long long s = 0;
        for (int j = 1; j <= k; j++) {
            long long term = e[k - j] * p[j] % MOD;
            if (j % 2 == 1) s = (s + term) % MOD;
            else s = (s - term + MOD) % MOD;
        }
        e[k] = s % MOD * power(k, MOD - 2, MOD) % MOD;
    }
    long long total = 0;
    for (int k = 1; k <= n; k++)
        total = (total + e[k]) % MOD;
    return total;
}

int main() {
    // Verify small cases
    vector<long long> test1 = {1, 2, 3};
    assert(solve_product(test1) == 23);
    assert(solve_newton(test1) == 23);

    vector<long long> test2 = {1, 2, 3, 4};
    assert(solve_product(test2) == 119);

    // Cross-check both methods
    vector<long long> test3 = {2, 3, 5, 7, 11};
    assert(solve_product(test3) == solve_newton(test3));

    // Solve main problem: x = (1, 2, ..., 1000)
    vector<long long> xs(1000);
    iota(xs.begin(), xs.end(), 1);
    cout << solve_product(xs) << endl;
    return 0;
}
