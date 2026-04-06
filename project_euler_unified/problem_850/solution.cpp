#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1e9 + 7;

ll power(ll b, ll e, ll m) { ll r=1; b%=m; while(e>0){if(e&1)r=r*b%m; b=b*b%m; e>>=1;} return r; }

ll catalan_mod(int n, ll mod) {
    ll num = 1, den = 1;
    for (int i = 2*n; i > n; i--) num = num * i % mod;
    for (int i = 1; i <= n; i++) den = den * i % mod;
    return num % mod * power(den, mod-2, mod) % mod * power(n+1, mod-2, mod) % mod;
}

int main() {
    // F(n) = C_{n-1}
    assert(catalan_mod(0, MOD) == 1);
    assert(catalan_mod(1, MOD) == 1);
    assert(catalan_mod(2, MOD) == 2);
    assert(catalan_mod(3, MOD) == 5);
    assert(catalan_mod(4, MOD) == 14);

    cout << catalan_mod(999, MOD) << endl;
    return 0;
}
