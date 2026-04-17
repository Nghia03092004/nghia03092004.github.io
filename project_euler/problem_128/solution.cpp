#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

ll mulmod(ll a, ll b, ll m) {
    return (lll)a * b % m;
}

ll powmod(ll a, ll b, ll m) {
    ll res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

bool miller_rabin(ll n, ll a) {
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

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23}) {
        if (!miller_rabin(n, a)) return false;
    }
    return true;
}

int main() {
    // Tile 1 is the first with PD=3.
    // For ring r >= 1:
    //   First tile S(r) = 3r(r-1)+2: check 6r-1, 6r+1, 12r+5 all prime
    //   Last tile  E(r) = 3r(r+1)+1: check 6r-1, 6r+5, 12r-7 all prime (r>=2)

    int count = 1; // tile 1
    int target = 2000;

    for (ll r = 1; ; r++) {
        // First tile of ring r
        if (is_prime(6*r - 1) && is_prime(6*r + 1) && is_prime(12*r + 5)) {
            count++;
            if (count == target) {
                cout << 3*r*(r-1) + 2 << endl;
                return 0;
            }
        }

        // Last tile of ring r (r >= 2 for 12r-7 to be > 1)
        if (r >= 2 && is_prime(6*r - 1) && is_prime(6*r + 5) && is_prime(12*r - 7)) {
            count++;
            if (count == target) {
                cout << 3*r*(r+1) + 1 << endl;
                return 0;
            }
        }
    }

    return 0;
}
