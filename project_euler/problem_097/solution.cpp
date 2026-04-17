#include <bits/stdc++.h>
using namespace std;

int main() {
    // Compute (28433 * 2^7830457 + 1) mod 10^10
    // Using binary exponentiation with __int128 to avoid overflow.

    const long long mod = 10000000000LL;

    auto mulmod = [](long long a, long long b, long long m) -> long long {
        return ((__int128)a * b) % m;
    };

    // Binary exponentiation: 2^7830457 mod 10^10
    long long base = 2, exp = 7830457, result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }

    result = mulmod(28433LL, result, mod);
    result = (result + 1) % mod;

    cout << result << endl;
    return 0;
}
