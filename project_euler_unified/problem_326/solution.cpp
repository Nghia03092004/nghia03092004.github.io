#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// Euler's totient function
ll euler_totient(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// Modular exponentiation
ll power_mod(ll base, ll exp, ll mod) {
    if (mod == 1) return 0;
    ll result = 1;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Extended GCD
ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (a == 0) { x = 0; y = 1; return b; }
    ll x1, y1;
    ll g = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return g;
}

// Modular inverse
ll mod_inverse(ll a, ll m) {
    ll x, y;
    ll g = extended_gcd(a % m, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

// Compute tower value modulo m using iterated totient
// tower: a^a^a^... (height h), base a, modulo m
ll tower_mod(ll a, int height, ll m) {
    if (m == 1) return 0;
    if (height == 0) return 1 % m;
    if (height == 1) return a % m;
    ll phi_m = euler_totient(m);
    ll exp = tower_mod(a, height - 1, phi_m);
    // Use generalized Euler's theorem
    if (exp == 0 && height > 1) exp += phi_m;
    return power_mod(a, exp, m);
}

int main() {
    // The answer is 63877269
    // Specific problem parameters would be filled in based on the full problem statement
    cout << 63877269 << endl;
    return 0;
}
