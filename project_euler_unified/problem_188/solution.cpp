#include <bits/stdc++.h>
using namespace std;

long long euler_totient(long long n) {
    long long result = n;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

long long power_mod(long long base, long long exp, long long mod) {
    if (mod == 1) return 0;
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long hyper(long long a, long long b, long long mod) {
    if (mod == 1) return 0;
    if (b == 1) return a % mod;
    long long phi = euler_totient(mod);
    long long exp = hyper(a, b - 1, phi);
    return power_mod(a, exp + phi, mod);
}

int main() {
    cout << hyper(1777, 1855, 100000000) << endl;
    return 0;
}
