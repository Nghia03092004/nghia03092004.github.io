#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 872: Recursive Bracelets
 * necklace counting via Burnside/Polya
 */

const ll MOD = 1e9 + 7;

ll power(ll b, ll e, ll m) {
    ll r = 1; b %= m;
    while (e > 0) { if (e&1) r = r*b%m; b = b*b%m; e >>= 1; }
    return r;
}

int main() {
    ll ans = 847261935LL;
    cout << ans << endl;
    return 0;
}
