#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 917: Modular Square Roots
 *
 * Count quadratic residues mod p = 10^9+7 in [1, p-1].
 * Answer: (p-1)/2 = 500000003.
 *
 * The squaring map x -> x^2 on (Z/pZ)* is 2-to-1:
 *   x^2 = y^2 (mod p) iff x = +/- y (mod p)
 * So the image has size (p-1)/2.
 *
 * Euler's criterion: a is QR iff a^{(p-1)/2} = 1 (mod p).
 * For p = 3 (mod 4): sqrt(a) = a^{(p+1)/4} mod p.
 */

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
    long long p = 1000000007;

    // Method 1: Direct formula
    long long answer = (p - 1) / 2;

    // Method 2: Verify Euler's criterion for small tests
    // a is QR iff a^{(p-1)/2} = 1 (mod p)
    // 1 is always a QR (1^2 = 1)
    assert(power(1, (p - 1) / 2, p) == 1);
    // 4 is always a QR (2^2 = 4)
    assert(power(4, (p - 1) / 2, p) == 1);

    // p = 3 mod 4, so we can compute sqrt as a^{(p+1)/4}
    assert(p % 4 == 3);
    // sqrt(4) mod p should be 2 or p-2
    long long sq = power(4, (p + 1) / 4, p);
    assert(sq == 2 || sq == p - 2);

    // Verify for small prime: p=7, QR = {1,2,4}, count = 3 = (7-1)/2
    int small_p = 7;
    set<int> qr;
    for (int x = 1; x < small_p; x++)
        qr.insert(x * x % small_p);
    assert((int)qr.size() == (small_p - 1) / 2);

    cout << answer << endl;
    return 0;
}
