#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 396: Weak Goodstein Sequence
 *
 * Find the last 9 digits of sum of G(n) for 1 <= n < 16.
 *
 * Key insight: G(n) can be computed via a hierarchical function decomposition.
 * The weak Goodstein sequence's digit-by-digit subtraction process maps to
 * iterated exponential (tetration-like) functions which can be evaluated
 * mod M using Euler's totient function for tower-of-powers modular arithmetic.
 *
 * f(0, t) = t + 1     (single digit decrement: t steps)
 * f(1, t) = 2t         (two-digit pattern)
 * f(2, t) = 2^t * t    (exponential growth)
 * f(3, t) = tower of 2s composed with itself t times
 *
 * G(n) is computed by decomposing n in binary and composing f functions.
 * Starting from t=3 (base offset), for each bit i set in n, apply f(i, t).
 * Then G(n) = result - 3.
 */

const int MOD = 1000000000;

int64_t fpm(int64_t b, int64_t e, int64_t m) {
    int64_t t = 1;
    b %= m;
    for (; e; e >>= 1, b = (__int128)b * b % m)
        if (e & 1) t = (__int128)t * b % m;
    return t;
}

// Euler's totient for numbers of form 10^9 / 2^a / 5^b
int phi_func(int x) {
    if (x % 2 == 0) x /= 2;
    if (x % 5 == 0) x = x / 5 * 4;
    return x;
}

struct node {
    int d; int64_t t, M;
    bool operator<(const node &o) const {
        if (d != o.d) return d < o.d;
        if (t != o.t) return t < o.t;
        return M < o.M;
    }
};

map<node, int> F;

// f3 computes the deeply recursive tower-of-powers function mod M
int f3(int d, int64_t t, int64_t M) {
    if (M == 1) return 0;
    node n{d, t, M};
    auto it = F.find(n);
    if (it != F.end()) return it->second;
    if (d == 0) return t % M;
    int64_t p = phi_func(M);
    int64_t x = f3(d - 1, t, p);
    return F[n] = (int)(fpm(2, x, M) * f3(d - 1, t, M) % M);
}

// f(x, t, M) computes the x-th level function mod M
int f(int x, int64_t t, int M) {
    if (M == 1) return 0;
    if (x == 0) return (t + 1) % M;
    if (x == 1) return t * 2 % M;
    if (x == 2) return fpm(2, t, M) * t % M;
    if (x == 3) return f3(t, t, M);
    assert(0);
    return 0;
}

// g(x) computes G(x) mod MOD using binary decomposition of x
int g(int x) {
    int t = 3;
    for (int i = 0; i < 5; i++) {
        if (x >> i & 1) {
            t = f(i, t, MOD);
        }
    }
    return t - 3;
}

int main() {
    int ans = 0;
    for (int i = 1; i <= 15; i++) {
        int gi = g(i);
        ans = (ans + gi) % MOD;
    }
    printf("%d\n", ans);
    return 0;
}
