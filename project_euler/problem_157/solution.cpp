#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Count number of positive divisors of n
int count_divisors(ll n) {
    if (n <= 0) return 0;
    int count = 0;
    for (ll d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            count++;
            if (d != n / d) count++;
        }
    }
    return count;
}

// Get all divisors of 10^(2n) = 2^(2n) * 5^(2n)
vector<ll> get_divisors(int n2) {
    vector<ll> divs;
    for (int a = 0; a <= n2; a++) {
        for (int b = 0; b <= n2; b++) {
            ll d = 1;
            for (int i = 0; i < a; i++) d *= 2;
            for (int i = 0; i < b; i++) d *= 5;
            divs.push_back(d);
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

int main() {
    ll total = 0;
    for (int n = 1; n <= 9; n++) {
        ll N = 1;
        for (int i = 0; i < n; i++) N *= 10;
        ll N2 = N * N;
        vector<ll> divs = get_divisors(2 * n);
        for (ll u : divs) {
            if (u > N) continue; // u <= N ensures a <= b
            ll v = N2 / u;
            ll g = __gcd(N + u, N + v);
            total += count_divisors(g);
        }
    }
    cout << total << endl;
    return 0;
}
