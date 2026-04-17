#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 258: A Lagged Fibonacci Sequence
 *
 * g(k) = g(k-2000) + g(k-1999) for k >= 2000
 * g(k) = 1 for 0 <= k <= 1999
 *
 * Find g(10^18) mod 20092010.
 *
 * Characteristic polynomial: x^2000 - x - 1 = 0
 * So x^2000 ≡ x + 1 (mod p(x))
 *
 * We compute x^(10^18) mod p(x) in Z_m[x], then:
 * g(10^18) = sum of coefficients a_i * g(i) = sum of a_i (since g(i)=1 for i<2000)
 *
 * Answer: 12747994
 */

const int N = 2000;
const long long MOD = 20092010;

typedef vector<long long> Poly;

// Reduce polynomial modulo p(x) = x^2000 - x - 1
// i.e., x^2000 ≡ x + 1
void reduce(Poly& a) {
    while ((int)a.size() > N) {
        long long coeff = a.back();
        int deg = a.size() - 1;
        a.pop_back();
        // x^deg = coeff * x^(deg-2000) * x^2000 = coeff * x^(deg-2000) * (x + 1)
        // = coeff * (x^(deg-1999) + x^(deg-2000))
        int base = deg - N;
        a[base] = (a[base] + coeff) % MOD;
        a[base + 1] = (a[base + 1] + coeff) % MOD;
    }
}

// Multiply two polynomials mod p(x) mod MOD
Poly polymul(const Poly& a, const Poly& b) {
    int sa = a.size(), sb = b.size();
    Poly c(sa + sb - 1, 0);
    for (int i = 0; i < sa; i++) {
        if (a[i] == 0) continue;
        for (int j = 0; j < sb; j++) {
            c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
        }
    }
    reduce(c);
    return c;
}

int main() {
    // Compute x^(10^18) mod p(x) mod 20092010
    long long exp = 1000000000000000000LL;

    // result = 1 (the polynomial "1")
    Poly result(1, 1);
    // base = x
    Poly base(2, 0);
    base[1] = 1;

    while (exp > 0) {
        if (exp & 1) {
            result = polymul(result, base);
        }
        base = polymul(base, base);
        exp >>= 1;
    }

    // g(10^18) = sum of a_i * g(i) where g(i) = 1 for 0 <= i <= 1999
    long long ans = 0;
    for (int i = 0; i < (int)result.size(); i++) {
        ans = (ans + result[i]) % MOD;
    }

    cout << ans << endl;
    return 0;
}
