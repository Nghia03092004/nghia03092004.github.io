#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 925: Continued Fraction Convergents
 *
 * sqrt(2) = [1; 2, 2, 2, ...]. Find p_100 + q_100 mod 10^9+7.
 *
 * Convergent recurrence: p_n = 2*p_{n-1} + p_{n-2}, same for q.
 * Initial: p_0=1, p_1=3, q_0=1, q_1=2.
 *
 * Key results:
 *   - Pell equation: p_n^2 - 2*q_n^2 = (-1)^{n+1}
 *   - Best approximation: |sqrt(2) - p_n/q_n| < 1/(q_n * q_{n+1})
 *   - Growth: q_n ~ (1+sqrt(2))^n / (2*sqrt(2))
 *   - Matrix form: [p_n; p_{n-1}] = [[2,1],[1,0]]^{n-1} * [3;1]
 *
 * Complexity: O(n) iterative, O(log n) via matrix exponentiation.
 */

int main() {
    long long MOD = 1000000007;
    long long p0 = 1, p1 = 3, q0 = 1, q1 = 2;
    for (int i = 2; i <= 100; i++) {
        long long np = (2 * p1 % MOD + p0) % MOD;
        long long nq = (2 * q1 % MOD + q0) % MOD;
        p0 = p1; p1 = np;
        q0 = q1; q1 = nq;
    }
    cout << (p1 + q1) % MOD << endl;

    // Verify Pell equation for small n (without mod)
    long long pp0 = 1, pp1 = 3, qq0 = 1, qq1 = 2;
    for (int i = 2; i <= 10; i++) {
        long long npp = 2*pp1 + pp0;
        long long nqq = 2*qq1 + qq0;
        pp0 = pp1; pp1 = npp;
        qq0 = qq1; qq1 = nqq;
    }
    // p_10^2 - 2*q_10^2 should be +/- 1
    assert(abs(pp1*pp1 - 2*qq1*qq1) == 1);

    return 0;
}
