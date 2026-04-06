#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1000000007;

ll sigma(ll n) {
    ll s = 0;
    for (ll d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            s += d;
            if (d != n / d) s += n / d;
        }
    }
    return s;
}

int main() {
    ll N = 10000;
    ll total = 0;
    for (ll n = 1; n <= N; n++)
        total = (total + sigma(n)) % MOD;
    cout << total << endl;
    return 0;
}
