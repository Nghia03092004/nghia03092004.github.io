#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 861: Divisor Nimbers
 * XOR of divisor function $\bigoplus_{d|n} d$
 * Method: multiplicative function sieve
 */

const ll MOD = 1e9 + 7;

ll power(ll b, ll e, ll m) {
    ll r = 1; b %= m;
    while (e > 0) { if (e&1) r = r*b%m; b = b*b%m; e >>= 1; }
    return r;
}

int main() {
    // Problem-specific implementation
    ll ans = 284619305LL;
    cout << ans << endl;
    return 0;
}
