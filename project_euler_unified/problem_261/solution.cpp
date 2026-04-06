#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * Problem 261: Pivotal Square Sums
 *
 * Reduction to generalized Pell equation X^2 - D*Y^2 = g,
 * where m = s*p^2, m+1 = g*r^2, D = s*g.
 * Iterate Pell solutions to recover pivots k = s*p*(p+q)/2.
 */

const ll LIMIT = 10000000000LL;
int spf_arr[100002];

void build_spf(int n) {
    for (int i = 0; i <= n; i++) spf_arr[i] = i;
    for (int i = 2; (ll)i * i <= n; i++)
        if (spf_arr[i] == i)
            for (int j = i * i; j <= n; j += i)
                if (spf_arr[j] == j) spf_arr[j] = i;
}

pair<int, int> sqfree_sqrt(int n) {
    int sf = 1, sq = 1;
    while (n > 1) {
        int p = spf_arr[n], e = 0;
        while (n % p == 0) { n /= p; e++; }
        if (e & 1) sf *= p;
        for (int i = 0; i < e / 2; i++) sq *= p;
    }
    return {sf, sq};
}

unordered_map<ll, pair<ll, ll>> pcache;

pair<ll, ll> pell_fund(ll D) {
    auto it = pcache.find(D);
    if (it != pcache.end()) return it->second;
    ll a0 = (ll)sqrtl((long double)D);
    while (a0 * a0 > D) a0--;
    while ((a0 + 1) * (a0 + 1) <= D) a0++;
    ll m = 0, d = 1, a = a0;
    ll n1 = 1, n0 = a0, d1 = 0, d0 = 1;
    for (int i = 0; i < 100000000; i++) {
        m = d * a - m;
        d = (D - m * m) / d;
        if (d == 0) break;
        a = (a0 + m) / d;
        ll n2 = n1, d2 = d1;
        n1 = n0; d1 = d0;
        n0 = a * n1 + n2; d0 = a * d1 + d2;
        if ((lll)n0 * n0 - (lll)D * d0 * d0 == 1) {
            pcache[D] = {n0, d0};
            return {n0, d0};
        }
        if (n0 > 4000000000000000000LL) break;
    }
    pcache[D] = {0, 0};
    return {0, 0};
}

int main() {
    int mmax = 70710;
    while (2LL * (mmax + 1) * ((ll)(mmax + 1) + 1) <= LIMIT) mmax++;

    build_spf(mmax + 2);
    unordered_set<ll> pivots;
    pivots.reserve(1 << 17);

    for (int m = 1; m <= mmax; m++) {
        auto [s, p] = sqfree_sqrt(m);
        auto [g, r] = sqfree_sqrt(m + 1);
        ll D = (ll)s * g;
        auto [x1, y1] = pell_fund(D);
        if (x1 == 0) continue;

        ll x = (ll)g * r, y = p;
        for (int iter = 0; iter < 100; iter++) {
            ll q = y;
            lll num = (lll)s * p * ((ll)p + q);
            if (num / 2 > LIMIT && q > p) break;
            if (num % 2 == 0) {
                ll k = (ll)(num / 2);
                if (k <= LIMIT && k >= 2LL * m * (m + 1)) {
                    ll u = x / g;
                    lll tv = (lll)g * r * u;
                    lll rem = tv - m - 1;
                    if (rem >= 0 && rem % 2 == 0) {
                        ll n_val = (ll)(rem / 2);
                        if (n_val >= k)
                            pivots.insert(k);
                    }
                }
            }
            lll xn = (lll)x * x1 + (lll)y * y1 * D;
            lll yn = (lll)x * y1 + (lll)y * x1;
            if (xn > (lll)4e18 || yn > (lll)4e18) break;
            x = (ll)xn;
            y = (ll)yn;
        }
    }

    ll ans = 0;
    for (ll k : pivots) ans += k;
    cout << ans << endl;
    return 0;
}
