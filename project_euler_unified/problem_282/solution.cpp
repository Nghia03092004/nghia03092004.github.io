#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

ll powmod(ll a, ll b, ll m) {
    if (m == 1) return 0;
    a %= m;
    if (a < 0) a += m;
    ll result = 1;
    while (b > 0) {
        if (b & 1) result = (lll)result * a % m;
        a = (lll)a * a % m;
        b >>= 1;
    }
    return result;
}

ll euler_phi(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

ll carmichael(ll n) {
    ll result = 1;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int e = 0;
            ll pk = 1;
            while (n % p == 0) { n /= p; e++; pk *= p; }
            ll lam = (p == 2 && e >= 3) ? pk / 4 : pk - pk / p;
            result = lcm(result, lam);
        }
    }
    if (n > 1) result = lcm(result, n - 1);
    return result;
}

// Compute 2^^height mod m via CRT on 2-part and odd-part
pair<ll, bool> tower2_mod(int height, ll m) {
    if (m == 1) return {0, true};
    if (height == 0) return {1 % m, 1 >= m};
    if (height == 1) return {2 % m, 2 >= m};

    int v2 = 0;
    ll modd = m;
    while (modd % 2 == 0) { modd /= 2; v2++; }

    // Mod 2^v2
    ll mod_2part = 0;
    if (v2 > 0) {
        ll exp_val;
        if (height - 1 <= 3) {
            ll small[] = {1, 2, 4, 16};
            exp_val = small[height - 1];
        } else {
            exp_val = 65536;
        }
        if (exp_val < v2)
            mod_2part = (1LL << exp_val) % (1LL << v2);
    }

    // Mod odd part
    ll mod_oddpart = 0;
    if (modd > 1) {
        ll lam = carmichael(modd);
        auto [exp_mod, _] = tower2_mod(height - 1, lam);
        mod_oddpart = powmod(2, exp_mod, modd);
    }

    // CRT
    ll result;
    if (v2 == 0) {
        result = mod_oddpart % m;
    } else {
        ll p2 = 1LL << v2;
        ll inv_p2 = powmod(p2 % modd, euler_phi(modd) - 1, modd);
        ll diff = ((mod_oddpart - mod_2part) % modd + modd) % modd;
        ll t = (lll)diff * inv_p2 % modd;
        result = mod_2part + p2 * t;
    }
    return {result % m, true};
}

int main() {
    ll M = 1;
    for (int i = 0; i < 8; i++) M *= 14;

    ll a0 = 1, a1 = 3, a2 = 7, a3 = 61;
    ll a4 = (tower2_mod(7, M).first - 3 + M) % M;
    ll stable = tower2_mod(100, M).first;
    ll a5 = (stable - 3 + M) % M;
    ll a6 = a5;

    cout << (a0 + a1 + a2 + a3 + a4 + a5 + a6) % M << endl;
    return 0;
}
