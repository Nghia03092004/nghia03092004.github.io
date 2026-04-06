#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll euler_totient(ll n) {
    ll result = n, temp = n;
    for (ll p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

ll power(ll b, ll e, ll m) {
    ll r = 1; b %= m;
    while (e > 0) { if (e&1) r = r*b%m; b = b*b%m; e >>= 1; }
    return r;
}

ll tetration_mod(ll a, ll n, ll m) {
    if (m == 1) return 0;
    if (n == 0) return 1 % m;
    if (n == 1) return a % m;
    ll phi = euler_totient(m);
    ll exp = tetration_mod(a, n-1, phi);
    if (__gcd(a, m) == 1) return power(a, exp, m);
    return power(a, exp + phi, m);
}

int main() {
    assert(tetration_mod(2, 3, 10) == 6);
    assert(tetration_mod(2, 4, 10) == 6);
    cout << tetration_mod(2, 100, 1000000007) << endl;
    return 0;
}
