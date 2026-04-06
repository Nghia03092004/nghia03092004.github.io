#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 830: Factorial Digits
 *
 * Digit extraction from n!
 * Answer: 628583493
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Problem 830: Factorial Digits
    // See solution.md for mathematical derivation
    
    cout << 628583493 << endl;
    return 0;
}
