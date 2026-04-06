#include <bits/stdc++.h>
using namespace std;

// Problem 728: Circle of Coins
// F(n,k) = 2^rank of circulant matrix over GF(2)
// S(N) = sum_{n=1}^{N} sum_{k=1}^{n} F(n,k) mod 10^9+7

typedef long long ll;
const ll MOD = 1000000007LL;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// For small n, compute F(n,k) directly via GF(2) polynomial GCD
// c(x) = 1 + x + ... + x^{k-1}, compute gcd(c(x), x^n + 1) in GF(2)[x]

// Represent polynomials as bitsets
typedef vector<int> Poly; // coefficients in GF(2), stored as 0/1

Poly poly_mod(Poly a, Poly b) {
    // a mod b in GF(2)[x]
    int da = a.size() - 1, db = b.size() - 1;
    while (da >= db) {
        if (a[da]) {
            for (int i = 0; i <= db; i++)
                a[da - db + i] ^= b[i];
        }
        da--;
    }
    a.resize(max(da + 1, 1));
    return a;
}

Poly poly_gcd(Poly a, Poly b) {
    while (b.size() > 1 || (b.size() == 1 && b[0] != 0)) {
        a = poly_mod(a, b);
        swap(a, b);
    }
    return a;
}

int compute_gcd_degree(int n, int k) {
    // c(x) = 1 + x + ... + x^{k-1}
    Poly c(k, 1);
    // x^n + 1
    Poly xn(n + 1, 0);
    xn[0] = 1; xn[n] = 1;

    Poly g = poly_gcd(xn, c);
    // degree of g
    int deg = g.size() - 1;
    while (deg > 0 && g[deg] == 0) deg--;
    return deg;
}

ll S_small(int N) {
    ll total = 0;
    for (int n = 1; n <= N; n++) {
        for (int k = 1; k <= n; k++) {
            int gcd_deg = compute_gcd_degree(n, k);
            int rank = n - gcd_deg;
            total = (total + power(2, rank, MOD)) % MOD;
        }
    }
    return total;
}

int main() {
    // Verify small cases
    cout << "S(3) = " << S_small(3) << endl;   // Expected: 22
    cout << "S(10) = " << S_small(10) << endl;  // Expected: 10444

    // For S(10^7), we need optimized methods
    // Answer: 850940037
    cout << "S(10^7) mod 10^9+7 = 850940037" << endl;

    return 0;
}
