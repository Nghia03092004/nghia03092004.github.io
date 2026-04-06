#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem: Find NF(61, 10^7) mod 61^10
    // NF(p,q) = v_p(N(p,q)!) where N(p,q) = sum T_n * p^n
    // Using Legendre: v_p(N!) = sum_{j=1}^{q} T_j * (1 + p + p^2 + ... + p^{j-1})
    // Mod p^e, the geometric sum caps at e terms.

    const long long p = 61;
    const int q = 10000000;
    const int e = 10;

    // Compute modulus = p^e
    long long mod = 1;
    for (int i = 0; i < e; i++) mod *= p;
    // mod = 61^10

    // Precompute geometric sums G[k] = 1 + p + p^2 + ... + p^{k-1} mod p^e
    // For k = 1..e
    vector<long long> G(e + 1, 0);
    long long pw = 1;
    for (int k = 1; k <= e; k++) {
        G[k] = (G[k - 1] + pw) % mod;
        if (k < e) pw = pw * p % mod;
    }

    // Generate T_n and accumulate
    long long s = 290797;
    long long result = 0;

    for (int n = 0; n <= q; n++) {
        long long t = s % p;
        if (n >= 1) {
            int idx = (n < e) ? n : e;
            result = (result + t * G[idx]) % mod;
        }
        // Advance RNG
        s = s * s % 50515093LL;
    }

    cout << result << endl;
    return 0;
}
