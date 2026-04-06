#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Modular multiplication using __int128 to avoid overflow.
ll mulmod(ll a, ll b, ll m) {
    return (__int128)a * b % m;
}

// Fast modular exponentiation: base^exp mod mod.
ll powmod(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// Single round of Miller-Rabin with witness a.
bool millerRabin(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ll x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

// Deterministic primality test for n < 3.2 * 10^18.
bool isPrime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll a : {2, 3, 5, 7, 11, 13}) {
        if (!millerRabin(n, a)) return false;
    }
    return true;
}

int main() {
    int primeCount = 0;
    int totalDiag = 1;  // center = 1

    for (ll k = 1; ; k++) {
        ll s = 2 * k + 1;
        ll sq = s * s;
        totalDiag += 4;

        // Three non-square corners: skip (2k+1)^2 by Lemma 1
        if (isPrime(sq - (s - 1)))     primeCount++;
        if (isPrime(sq - 2 * (s - 1))) primeCount++;
        if (isPrime(sq - 3 * (s - 1))) primeCount++;

        if (primeCount > 0 && 10 * primeCount < totalDiag) {
            cout << s << endl;
            return 0;
        }
    }
    return 0;
}
