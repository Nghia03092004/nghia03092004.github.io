#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 409: Nim Extreme
 *
 * W(n) = n! * (2^n - 1) / 2^n * (C(2^n-1, n) - E_n)  mod M
 *
 * where E_n = (-1)^h * C(2^{n-1}-1, h),  h = n/2      (n even)
 *       E_n = (-1)^{(n+1)/2} * C(2^{n-1}-1, (n-1)/2)  (n odd)
 *
 * All modular arithmetic with M = 10^9 + 7.
 */

static const long long M = 1000000007LL;

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

long long solve(int n) {
    long long pow2n  = power(2, n, M);
    long long pow2n1 = power(2, n - 1, M);
    long long Nm1    = (pow2n - 1 + M) % M;    // 2^n - 1
    long long m_mod  = (pow2n1 - 1 + M) % M;   // 2^{n-1} - 1

    // Compute n! and C(2^n-1, n) = falling_factorial(Nm1, n) / n!
    long long fact_n = 1, falling_Nm1 = 1;
    for (int i = 0; i < n; i++) {
        fact_n = fact_n * ((i + 1) % M) % M;
        falling_Nm1 = falling_Nm1 % M * ((Nm1 - i % M + M) % M) % M;
    }
    long long inv_fact_n = modinv(fact_n, M);
    long long C_Nm1_n = falling_Nm1 % M * inv_fact_n % M;

    // Compute C(m, h) where m = 2^{n-1}-1, h = n/2
    int h = n / 2;
    long long fact_h = 1, falling_m = 1;
    for (int i = 0; i < h; i++) {
        fact_h = fact_h * ((i + 1) % M) % M;
        falling_m = falling_m % M * ((m_mod - i % M + M) % M) % M;
    }
    long long inv_fact_h = modinv(fact_h, M);
    long long C_m_h = falling_m % M * inv_fact_h % M;

    // E_n with sign
    int sign;
    if (n % 2 == 0) {
        sign = (h % 2 == 0) ? 1 : -1;
    } else {
        sign = (((n + 1) / 2) % 2 == 0) ? 1 : -1;
    }
    long long E_n_mod = (sign == 1) ? C_m_h : (M - C_m_h) % M;

    // W(n) = n! * (2^n - 1) * inv(2^n) * (C(2^n-1,n) - E_n) mod M
    long long inv_pow2n = modinv(pow2n, M);
    long long W = fact_n;
    W = W % M * (Nm1 % M) % M;
    W = W % M * (inv_pow2n % M) % M;
    W = W % M * ((C_Nm1_n - E_n_mod + M) % M) % M;
    return W;
}

int main() {
    ios_base::sync_with_stdio(false);

    // Verification
    struct { int n; long long expected; } tests[] = {
        {1, 1}, {2, 6}, {3, 168}, {5, 19764360}, {100, 384777056}
    };
    cout << "=== Verification ===" << endl;
    for (auto& t : tests) {
        long long w = solve(t.n);
        cout << "  W(" << t.n << ") mod M = " << w
             << "  expected " << t.expected
             << "  [" << (w == t.expected ? "OK" : "FAIL") << "]" << endl;
    }

    // Main computation
    cout << "\n=== Main computation ===" << endl;
    auto t0 = chrono::steady_clock::now();
    long long answer = solve(10000000);
    auto t1 = chrono::steady_clock::now();
    double elapsed = chrono::duration<double>(t1 - t0).count();

    cout << "  W(10,000,000) mod 10^9+7 = " << answer << endl;
    cout << "  Time: " << fixed << setprecision(2) << elapsed << "s" << endl;

    return 0;
}
