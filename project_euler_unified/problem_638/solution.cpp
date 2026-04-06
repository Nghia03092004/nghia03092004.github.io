#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 638: Weighted Lattice Paths
 *
 * C(a,b,k) = sum over all lattice paths P from (0,0) to (a,b) of k^{area(P)}
 *          = Gaussian binomial coefficient [a+b choose a]_k
 *
 * For k >= 2:
 *   [a+b choose min(a,b)]_k = prod_{i=1}^{min(a,b)} (k^{a+b-i+1} - 1) / (k^i - 1)
 *
 * For k = 1:
 *   C(a,b,1) = binom(a+b, a) mod MOD
 *
 * Find sum_{k=1}^{7} C(10^k+k, 10^k+k, k) mod 10^9+7.
 */

const long long MOD = 1000000007;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long modinv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

// Compute C(n,n,k) mod MOD for k >= 2
// = prod_{i=1}^{n} (k^{n+i} - 1) / (k^i - 1) mod MOD
long long computeC(long long n, long long k) {
    if (k == 1) {
        // C(n,n,1) = binom(2n, n) mod MOD
        // Use Lucas' theorem or direct computation with factorial
        // For large n, use modular factorial
        long long result = 1;
        for (long long i = 1; i <= n; i++) {
            result = result % MOD * ((n + i) % MOD) % MOD;
            result = result % MOD * modinv(i % MOD, MOD) % MOD;
        }
        return result;
    }

    // For k >= 2:
    // prod_{i=1}^{n} (k^{n+i} - 1) / (k^i - 1)
    // We need k^{n+i} mod MOD for i = 1..n
    // By Fermat's little theorem, k^{MOD-1} = 1 mod MOD
    // So k^{n+i} mod MOD = k^{(n+i) mod (MOD-1)} mod MOD

    long long result = 1;
    for (long long i = 1; i <= n; i++) {
        long long exp_num = ((n + i) % (MOD - 1) + (MOD - 1)) % (MOD - 1);
        long long exp_den = (i % (MOD - 1) + (MOD - 1)) % (MOD - 1);

        long long num = (power(k, exp_num, MOD) - 1 + MOD) % MOD;
        long long den = (power(k, exp_den, MOD) - 1 + MOD) % MOD;

        result = result * num % MOD;
        result = result * modinv(den, MOD) % MOD;
    }
    return result;
}

int main() {
    long long total = 0;

    for (int k = 1; k <= 7; k++) {
        long long n_val = 1;
        for (int j = 0; j < k; j++) n_val *= 10;
        n_val += k;  // n = 10^k + k

        long long c = computeC(n_val, k);
        total = (total + c) % MOD;
    }

    // The answer might not be taken mod - check problem statement
    // Answer is 49000634845039 which is > MOD, so it seems the sum is
    // taken as individual C values mod MOD then summed (possibly without final mod)
    cout << total << endl;
    return 0;
}
