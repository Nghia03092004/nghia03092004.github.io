#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 834: Sum of Concatenations
 *
 * Digit concatenation arithmetic
 * Answer: 472780589
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

// S(N) = (N(N+1)/2) * T + N * (N(N+1)/2)
// where T = sum_{m=1}^{N} 10^{d(m)}

int main() {
    long long N = 1000000000000000000LL;
    long long inv2 = modinv(2, MOD);
    long long sum_n = N % MOD * ((N + 1) % MOD) % MOD * inv2 % MOD;
    
    // T = sum 10^{d(m)} for m=1..N
    long long T = 0;
    long long lo = 1;
    int d = 1;
    while (lo <= N) {
        long long hi = min(lo * 10 - 1, N);
        long long count = (hi - lo + 1) % MOD;
        T = (T + count * power(10, d, MOD)) % MOD;
        lo *= 10;
        d++;
    }
    
    long long ans = (sum_n * T + N % MOD * sum_n) % MOD;
    cout << ans << endl;
    return 0;
}
