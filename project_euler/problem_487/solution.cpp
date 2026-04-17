#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

ll mod_inv(ll a, ll p) {
    return mod_pow(a, p - 2, p);
}

// Compute Bernoulli numbers B[0..k] mod p
vector<ll> bernoulli_mod(int k, ll p) {
    vector<ll> B(k + 1, 0);
    B[0] = 1;
    for (int m = 1; m <= k; m++) {
        ll s = 0;
        ll c = 1; // C(m+1, j)
        for (int j = 0; j < m; j++) {
            s = (s + c % p * B[j]) % p;
            c = c % p * ((m + 1 - j) % p) % p * mod_inv(j + 1, p) % p;
        }
        B[m] = (p - s % p) % p * mod_inv(m + 1, p) % p;
    }
    return B;
}

// Compute S_k(p) = sum_{i=1}^{p-1} f_k(i) mod p
// Using Faulhaber + Fermat's Little Theorem
ll sum_power_sums(int k, ll p) {
    auto B = bernoulli_mod(k, p);
    ll inv_k1 = mod_inv(k + 1, p);
    ll result = 0;
    ll c = 1; // C(k+1, j)

    for (int j = 0; j <= k; j++) {
        int m = k + 1 - j;
        ll inner;
        if (m == 0) {
            inner = (p - 1) % p;
        } else if (m % (p - 1) == 0) {
            inner = p - 1; // = -1 mod p
        } else {
            inner = 0;
        }
        result = (result + c % p * (B[j] % p) % p * inner) % p;
        c = c % p * ((k + 1 - j) % p) % p * mod_inv(j + 1, p) % p;
    }

    return result % p * inv_k1 % p;
}

int main() {
    // Demonstration: compute S_k(p) for small primes and k values
    vector<ll> primes = {5, 7, 11, 13, 17, 19, 23};
    for (ll p : primes) {
        for (int k = 1; k <= 4; k++) {
            ll val = sum_power_sums(k, p);
            cout << "S_" << k << "(" << p << ") = " << val << endl;
        }
    }
    return 0;
}
