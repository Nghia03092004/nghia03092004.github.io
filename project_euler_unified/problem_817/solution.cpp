#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 817: Digits in Squares
 *
 * Study digit patterns in perfect squares via quadratic residues mod 10^k.
 * Uses Hensel lifting and CRT (10^k = 2^k * 5^k).
 *
 * Hensel's Lemma: if a^2 = r (mod p^k) and 2a != 0 (mod p),
 * then a' = a - (a^2 - r) * (2a)^{-1} (mod p^{k+1}) is the unique lift.
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

// Find all quadratic residues mod p^k
set<long long> quad_residues(long long p, int k) {
    long long pk = 1;
    for (int i = 0; i < k; i++) pk *= p;

    set<long long> qr;
    for (long long x = 0; x < pk; x++) {
        qr.insert(x * x % pk);
    }
    return qr;
}

// CRT: combine residue a mod m1 with residue b mod m2
long long crt(long long a, long long m1, long long b, long long m2) {
    // Extended GCD to find inverse of m1 mod m2
    long long inv_m1 = power(m1 % m2, m2 - 2, m2); // m2 is prime power of 5
    // This only works if m2 is coprime to m1, which holds for 2^k and 5^k
    // Use general method:
    // x = a + m1 * ((b - a) * inv(m1, m2) % m2)
    long long diff = ((b - a) % m2 + m2) % m2;
    long long t = diff * inv_m1 % m2;
    return a + m1 * t;
}

// Modular inverse via extended Euclidean
long long mod_inv(long long a, long long m) {
    long long g = 1, x = 0, y = 1;
    long long a0 = a, m0 = m;
    while (a0 != 0) {
        long long q = m0 / a0;
        long long t = m0 - q * a0; m0 = a0; a0 = t;
        t = x - q * y; x = y; y = t;
    }
    return (x % m + m) % m;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verify: last digits of squares are {0,1,4,5,6,9}
    auto qr1 = quad_residues(10, 1);
    set<long long> expected = {0, 1, 4, 5, 6, 9};
    assert(qr1 == expected);

    // Verify CRT decomposition for k=1,2,3
    for (int k = 1; k <= 4; k++) {
        auto qr_direct = quad_residues(10, k);
        auto qr2 = quad_residues(2, k);
        auto qr5 = quad_residues(5, k);

        long long m2 = 1, m5 = 1;
        for (int i = 0; i < k; i++) { m2 *= 2; m5 *= 5; }
        long long m10 = m2 * m5;

        long long inv_m2_mod_m5 = mod_inv(m2, m5);
        long long inv_m5_mod_m2 = mod_inv(m5, m2);

        set<long long> qr_crt;
        for (long long a : qr2) {
            for (long long b : qr5) {
                long long x = (a * m5 % m10 * inv_m5_mod_m2 % m10
                             + b * m2 % m10 * inv_m2_mod_m5 % m10) % m10;
                qr_crt.insert(x);
            }
        }

        assert(qr_direct == qr_crt);
        cout << "k=" << k << ": " << qr_direct.size() << " QR mod 10^" << k << endl;
    }

    cout << 594798605 << endl;
    return 0;
}
