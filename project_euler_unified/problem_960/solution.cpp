/*
 * Problem 960: Modular Combinatorics
 *
 * Compute sum_{k=0}^{n} C(n,k)*k^3 mod (10^9+7), n = 10^6.
 *
 * Using Stirling numbers: k^3 = k + 3k(k-1) + k(k-1)(k-2)
 * sum C(n,k)*k^{r_down} = n^{r_down} * 2^{n-r}
 *
 * Formula: n*2^{n-1} + 3*n*(n-1)*2^{n-2} + n*(n-1)*(n-2)*2^{n-3} mod p
 *
 * Complexity: O(log n) for modular exponentiation.
 */

#include <bits/stdc++.h>
using namespace std;

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

int main() {
    long long n = 1000000;
    long long p = MOD;

    long long pow2_n1 = power(2, n - 1, p);
    long long pow2_n2 = power(2, n - 2, p);
    long long pow2_n3 = power(2, n - 3, p);

    long long nn = n % p;
    long long nn1 = (n - 1) % p;
    long long nn2 = (n - 2) % p;

    long long term1 = nn * pow2_n1 % p;
    long long term2 = 3 * nn % p * nn1 % p * pow2_n2 % p;
    long long term3 = nn * nn1 % p * nn2 % p * pow2_n3 % p;

    long long ans = (term1 + term2 + term3) % p;
    cout << ans << endl;
    return 0;
}
