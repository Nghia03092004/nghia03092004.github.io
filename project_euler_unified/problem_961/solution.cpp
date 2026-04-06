/*
 * Problem 961: Repunit Divisibility
 *
 * Find sum of n <= 1000 such that n | R_n where R_n = (10^n - 1)/9.
 * Equivalently: 10^n = 1 (mod 9n), for gcd(n, 10) = 1.
 *
 * Complexity: O(N log N) with binary exponentiation.
 */

#include <bits/stdc++.h>
using namespace std;

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
    const int N = 1000;
    long long total = 0;

    for (int n = 1; n <= N; n++) {
        if (n % 2 == 0 || n % 5 == 0) continue;  // gcd(n,10) > 1
        long long mod = 9LL * n;
        if (power(10, n, mod) == 1) {
            total += n;
        }
    }

    // n=1 is special: R_1 = 1, 1|1
    // Already handled since gcd(1,10)=1 and 10^1 mod 9 = 1

    cout << total << endl;
    return 0;
}
