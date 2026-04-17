#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 848: Fraction Field
 *
 * Continued fraction expansion via Euclidean algorithm.
 * Convergent recurrence: p_k = a_k * p_{k-1} + p_{k-2}.
 */

const ll MOD = 1e9 + 7;

vector<ll> cf_expansion(ll p, ll q) {
    vector<ll> cf;
    while (q != 0) {
        cf.push_back(p / q);
        ll r = p % q;
        p = q; q = r;
    }
    return cf;
}

vector<pair<ll,ll>> convergents(const vector<ll>& cf) {
    vector<pair<ll,ll>> convs;
    ll p2 = 0, p1 = 1, q2 = 1, q1 = 0;
    for (ll a : cf) {
        ll p = a * p1 + p2;
        ll q = a * q1 + q2;
        convs.push_back({p, q});
        p2 = p1; p1 = p;
        q2 = q1; q1 = q;
    }
    return convs;
}

int main() {
    // Verify 355/113 = [3; 7, 16]
    auto cf = cf_expansion(355, 113);
    assert(cf.size() == 3 && cf[0] == 3 && cf[1] == 7 && cf[2] == 16);

    auto convs = convergents(cf);
    assert(convs.back().first == 355 && convs.back().second == 113);
    assert(convs[1].first == 22 && convs[1].second == 7);

    // Determinant identity check
    for (int k = 1; k < (int)convs.size(); k++) {
        ll det = convs[k].first * convs[k-1].second
               - convs[k-1].first * convs[k].second;
        assert(abs(det) == 1);
    }

    // Compute answer
    ll total = 0;
    for (ll n = 1; n <= 10000; n++) {
        auto cf2 = cf_expansion(n * n + 1, n);
        auto conv2 = convergents(cf2);
        for (auto [p, q] : conv2)
            total = (total + q % MOD) % MOD;
    }
    cout << total << endl;
    return 0;
}
