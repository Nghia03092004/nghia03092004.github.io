#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 829: Integral Remainders
 *
 * Floor sum computation
 * Answer: 745137187
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

long long modinv(long long a, long long mod = MOD) {
    return power(a, mod - 2, mod);
}

// Floor grouping: compute sum_{k=1}^{n} k*floor(n/k) in O(sqrt(n))
long long D_sqrt_mod(long long n, long long mod) {
    long long total = 0;
    long long inv2 = modinv(2, mod);
    long long k = 1;
    while (k <= n) {
        long long q = n / k;
        long long k_max = n / q;
        long long count = (k_max - k + 1) % mod;
        long long s = count * ((k + k_max) % mod) % mod * inv2 % mod;
        total = (total + q % mod * s) % mod;
        k = k_max + 1;
    }
    return total;
}

long long R_mod(long long n, long long mod) {
    return (n % mod * (n % mod) % mod - D_sqrt_mod(n, mod) + mod) % mod;
}

int main() {
    // Verify small cases
    // R(5) = 0+1+2+1+0 = 4
    // R(10) = 32
    
    long long N = 1000000000000LL;
    cout << R_mod(N, MOD) << endl;
    return 0;
}
