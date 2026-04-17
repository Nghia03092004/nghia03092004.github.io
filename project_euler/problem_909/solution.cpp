#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 909: Random Walk Return Probability
 *
 * R(2n) = C(2n, n) / 4^n.  For n = 10: R(20) = 184756 / 1048576.
 * Reduce to lowest terms: 46189 / 262144.  Answer: p + q = 308333.
 *
 * Two methods:
 *   1. Direct computation with gcd reduction
 *   2. Iterative C(2n,n) with 2-adic valuation tracking
 *
 * Key result: R(2n) ~ 1/sqrt(pi*n) (Stirling). The walk on Z is recurrent
 * because sum R(2n) diverges.
 */

int main() {
    int n = 10;

    // Compute C(2n, n)
    long long num = 1;
    for (int i = 1; i <= n; i++) {
        num *= (n + i);
        num /= i;
    }
    // num = C(20, 10) = 184756

    long long den = 1LL << (2 * n);  // 4^n = 2^{2n} = 1048576

    long long g = __gcd(num, den);
    long long p = num / g;  // 46189
    long long q = den / g;  // 262144

    // Verify
    assert(p == 46189);
    assert(q == 262144);
    assert(__gcd(p, q) == 1);  // fully reduced

    cout << p + q << endl;  // 308333

    // Verify small cases
    // R(2) = C(2,1)/4 = 2/4 = 1/2, p+q = 3
    assert(__gcd(2LL, 4LL) == 2);

    // R(4) = C(4,2)/16 = 6/16 = 3/8, p+q = 11
    long long g2 = __gcd(6LL, 16LL);
    assert(6 / g2 == 3 && 16 / g2 == 8);

    return 0;
}
