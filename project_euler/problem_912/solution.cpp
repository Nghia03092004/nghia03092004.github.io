#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 912: Polynomial Roots over Finite Fields
 *
 * For p = 997 and f(x) = x^3 + 2x + 1, count roots in F_p.
 *
 * By Lagrange's theorem: at most 3 roots (degree 3 polynomial).
 * By discriminant analysis: Delta = -4*2^3 - 27*1^2 = -59.
 * The Legendre symbol (Delta/p) determines root structure.
 *
 * Two methods:
 *   1. Brute-force: evaluate f(x) for all x in [0, p-1]
 *   2. Horner's method for efficient polynomial evaluation
 */

int main() {
    int p = 997;

    // Method 1: Direct evaluation with Horner's method
    // f(x) = x^3 + 2x + 1 = x*(x*x + 0*x + 2) + 1
    // Horner: ((x)*x + 0)*x + 2*x + 1
    // Better: coeffs [1, 0, 2, 1]: val = ((1*x + 0)*x + 2)*x + 1

    vector<int> roots;
    for (int x = 0; x < p; x++) {
        // Horner: f(x) = ((x)*x + 0)*x + 2*x + 1
        long long val = x;
        val = (val * x) % p;       // x^2
        val = (val + 2) % p;       // x^2 + 2
        val = (val * x + 1) % p;   // x^3 + 2x + 1
        if (val == 0) {
            roots.push_back(x);
        }
    }

    // Method 2: Discriminant analysis
    // Delta = -4*8 - 27*1 = -59
    // Legendre symbol (-59 / 997)
    int delta = ((-59 % p) + p) % p;
    long long leg = 1;
    {
        long long a = delta, e = (p - 1) / 2, r = 1;
        while (e > 0) {
            if (e & 1) r = r * a % p;
            a = a * a % p;
            e >>= 1;
        }
        leg = r;
    }
    // If leg == 1: Delta is QR => 0 or 3 roots
    // If leg == p-1: Delta is QNR => exactly 1 root

    // Verify each root
    for (int r : roots) {
        long long val = ((long long)r * r % p * r % p + 2 * r + 1) % p;
        assert(val == 0);
    }

    cout << (int)roots.size() << endl;
    return 0;
}
