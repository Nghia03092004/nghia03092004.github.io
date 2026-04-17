#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

/*
 * Problem 531: Chinese Leftovers
 *
 * Compute S = sum of g(phi(n), n, phi(m), m) over 1000000 <= n < m <= 1005000.
 * g(a,n,b,m) = smallest non-negative x with x=a(mod n), x=b(mod m), or 0.
 *
 * Method: Euler totient sieve + extended CRT via Bezout's identity.
 * Time: O(N log log N + K^2 log N).
 */

ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (a == 0) { x = 0; y = 1; return b; }
    ll x1, y1;
    ll g = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return g;
}

ll crt(ll a, ll n, ll b, ll m) {
    ll u, v;
    ll g = extended_gcd(n, m, u, v);
    if ((a - b) % g != 0) return 0;
    ll mg = m / g;
    ll diff = ((lll)(b - a) / g % mg * u % mg + mg) % mg;
    ll lcm = n / g * m;
    ll x = (lll)a + (lll)n * diff;
    x = ((x % lcm) + lcm) % lcm;
    return x;
}

int main() {
    const int LO = 1000000, HI = 1005000;
    vector<ll> phi(HI + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int i = 2; i <= HI; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= HI; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    ll S = 0;
    for (int n = LO; n <= HI; n++)
        for (int m = n + 1; m <= HI; m++)
            S += crt(phi[n], n, phi[m], m);

    cout << S << endl;
    return 0;
}
