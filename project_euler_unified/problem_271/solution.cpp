#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

// Extended GCD
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// Modular inverse of a mod m
ll modinv(ll a, ll m) {
    ll x, y;
    extgcd(a, m, x, y);
    return ((x % m) + m) % m;
}

// Find cube roots of 1 mod p
vector<ll> cube_roots_of_unity(ll p) {
    vector<ll> roots;
    roots.push_back(1);
    if (p <= 3) return roots;
    if ((p - 1) % 3 != 0) return roots;
    // Find roots of x^2 + x + 1 = 0 mod p
    // x = (-1 +/- sqrt(-3)) / 2 mod p
    // Find sqrt(-3) mod p
    ll neg3 = p - 3;
    // Tonelli-Shanks for sqrt(neg3) mod p
    // Since p is small, just brute force
    ll sq = -1;
    for (ll i = 0; i < p; i++) {
        if ((i * i) % p == neg3) { sq = i; break; }
    }
    if (sq == -1) return roots;
    ll inv2 = modinv(2, p);
    ll r1 = (((p - 1) + sq) % p * inv2) % p;
    ll r2 = (((p - 1) + (p - sq)) % p * inv2) % p;
    if (r1 != 1) roots.push_back(r1);
    if (r2 != 1) roots.push_back(r2);
    return roots;
}

int main() {
    vector<ll> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43};
    ll N = 1;
    for (ll p : primes) N *= p;
    // N = 13082761331670030

    // For each prime, find cube roots of unity
    vector<vector<ll>> roots(14);
    for (int i = 0; i < 14; i++) {
        roots[i] = cube_roots_of_unity(primes[i]);
    }

    // CRT: for each combination, reconstruct n mod N
    // Precompute CRT coefficients: M_i = N / p_i, and M_i_inv = inverse of M_i mod p_i
    vector<ll> M(14), Minv(14);
    for (int i = 0; i < 14; i++) {
        M[i] = N / primes[i];
        Minv[i] = modinv(M[i] % primes[i], primes[i]);
    }

    // Enumerate all combinations using recursion
    // Use __int128 for intermediate computations
    lll total_sum = 0;
    int count = 0;

    // Iterative enumeration
    vector<int> idx(14, 0);
    while (true) {
        // Compute n via CRT
        lll n = 0;
        for (int i = 0; i < 14; i++) {
            lll term = (lll)roots[i][idx[i]] * Minv[i] % primes[i];
            term = term * M[i];
            n += term;
        }
        n %= (lll)N;
        if (n <= 0) n += N;
        if (n > 1 && n < N) {
            total_sum += n;
            count++;
        }

        // Increment indices
        int pos = 13;
        while (pos >= 0) {
            idx[pos]++;
            if (idx[pos] < (int)roots[pos].size()) break;
            idx[pos] = 0;
            pos--;
        }
        if (pos < 0) break;
    }

    // Print result
    // __int128 printing
    lll ans = total_sum;
    string s;
    if (ans == 0) s = "0";
    else {
        while (ans > 0) {
            s += ('0' + (int)(ans % 10));
            ans /= 10;
        }
        reverse(s.begin(), s.end());
    }
    cout << s << endl;
    return 0;
}
