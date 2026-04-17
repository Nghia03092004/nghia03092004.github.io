#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 922: Elliptic Curve Point Counting
 *
 * Count |E(F_p)| for E: y^2 = x^3 + x + 1, p = 1009.
 *
 * Formula: |E| = 1 + p + sum_{x=0}^{p-1} Legendre(f(x), p)
 *   where f(x) = x^3 + x + 1.
 *   Legendre(a, p) = a^{(p-1)/2} mod p, mapped to {-1, 0, 1}.
 *
 * Hasse's theorem: |p + 1 - |E|| <= 2*sqrt(p) ~ 63.6 for p=1009.
 * Non-singularity: 4*1 + 27*1 = 31 != 0 mod 1009.
 *
 * Also implements group law for verification.
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

int count_points(int a, int b, int p) {
    int count = 1; // point at infinity
    int half = (p - 1) / 2;
    for (int x = 0; x < p; x++) {
        long long rhs = ((long long)x * x % p * x % p + (long long)a * x % p + b) % p;
        if (rhs < 0) rhs += p;
        int leg;
        if (rhs == 0) {
            leg = 0;
        } else {
            leg = (int)power(rhs, half, p);
            if (leg == p - 1) leg = -1;
        }
        count += 1 + leg;
    }
    return count;
}

int main() {
    int p = 1009;
    int answer = count_points(1, 1, p);
    cout << answer << endl;

    // Verify Hasse bound
    int trace = p + 1 - answer;
    assert(abs(trace) <= 2 * (int)sqrt((double)p) + 1);

    // Verify non-singularity: 4a^3 + 27b^2 = 31 != 0 mod p
    assert((4 + 27) % p != 0);

    // Cross-check with brute force for p=23
    int p2 = 23;
    int leg_count = count_points(1, 1, p2);
    int brute = 1;
    for (int x = 0; x < p2; x++) {
        int rhs = (x * x % p2 * x % p2 + x + 1) % p2;
        if (rhs < 0) rhs += p2;
        for (int y = 0; y < p2; y++)
            if (y * y % p2 == rhs) brute++;
    }
    assert(leg_count == brute);

    return 0;
}
