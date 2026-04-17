#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

const ll MOD = 282475249LL; // 7^10 (NOT prime!)

ll md(lll x) {
    ll r = (ll)(x % (lll)MOD);
    return r < 0 ? r + MOD : r;
}

// Extended GCD for modular inverse (MOD is not prime, can't use Fermat)
ll modinv(ll a, ll m) {
    ll g = m, x = 0, y = 1;
    ll aa = a % m;
    if (aa < 0) aa += m;
    while (aa != 0) {
        ll q = g / aa;
        g -= q * aa; swap(g, aa);
        x -= q * y; swap(x, y);
    }
    return (x % m + m) % m;
}

const ll inv2 = modinv(2, MOD);
const ll inv6 = modinv(6, MOD);

ll mul(ll a, ll b) { return md((lll)a * b); }
ll add(ll a, ll b) { return (a + b) % MOD; }
ll sub(ll a, ll b) { return (a - b % MOD + MOD) % MOD; }

ll S1n(lll n) { return mul(mul(md(n), md(n - 1)), inv2); }
ll S2n(lll n) { return mul(mul(mul(md(n), md(n - 1)), md(2 * n - 1)), inv6); }

struct T3 { ll f0, f1, f2; };

T3 fsq(lll N, lll a, lll b, lll m) {
    if (N <= 0) return {0, 0, 0};

    if (a >= m) {
        lll qa = a / m;
        auto g = fsq(N, a % m, b, m);
        ll s1 = S1n(N), s2 = S2n(N);
        ll qm = md(qa);
        ll f0 = add(mul(qm, s1), g.f0);
        ll f1 = add(mul(qm, s2), g.f1);
        ll f2 = add(add(mul(mul(qm, qm), s2), mul(mul(2, qm), g.f1)), g.f2);
        return {f0, f1, f2};
    }

    if (b >= m) {
        lll qb = b / m;
        auto g = fsq(N, a, b % m, m);
        ll qm = md(qb), nm = md(N);
        ll f0 = add(mul(qm, nm), g.f0);
        ll f1 = add(mul(qm, S1n(N)), g.f1);
        ll f2 = add(add(mul(mul(qm, qm), nm), mul(mul(2, qm), g.f0)), g.f2);
        return {f0, f1, f2};
    }

    if (a == 0) return {0, 0, 0};

    lll Mv = (a * (N - 1) + b) / m;
    if (Mv == 0) return {0, 0, 0};

    auto g = fsq(Mv, m, m - b - 1, a);

    ll nm = md(N - 1), mm = md(Mv), T = S1n(N);

    ll f0 = sub(mul(nm, mm), g.f0);
    ll f1 = sub(mul(mm, T), mul(inv2, add(g.f2, g.f0)));
    ll f2 = sub(sub(mul(mul(nm, mm), mm), mul(2, g.f1)), g.f0);

    return {f0, f1, f2};
}

ll isqrt_safe(lll n) {
    if (n <= 0) return 0;
    lll s = (lll)sqrtl((long double)n);
    while (s > 0 && s * s > n) s--;
    while ((s + 1) * (s + 1) <= n) s++;
    return (ll)s;
}

ll Lf(ll a) {
    lll n = (lll)a * a * 5;
    ll s = isqrt_safe(n);
    return (s - a) / 2;
}

int main() {
    ll N = 10000000000000000LL; // 10^16
    lll P = 14472334024676221LL; // F_79
    lll Q = 23416728348467685LL; // F_80

    // Binary search for a_star
    ll lo = 1, hi = N - 1;
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;
        if (Lf(mid) <= N - mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    ll a_star = lo;

    // Range 1: a = 1..a_star, R(a) = L(a)
    auto r = fsq((lll)a_star + 1, P, 0, Q);
    ll range1 = add(mul(2, r.f1), mul(inv2, add(r.f2, r.f0)));

    // Range 2: a = a_star+1..N-1, R(a) = N-a
    ll J = N - a_star - 1;
    ll range2 = 0;
    if (J > 0) {
        ll jm = md(J), j1 = md(J+1), j2 = md(2*J+1), nm = md(N);
        ll sj = mul(mul(jm, j1), inv2);
        ll sj2 = mul(mul(mul(jm, j1), j2), inv6);
        range2 = sub(mul(add(mul(2, nm), inv2), sj), mul(mul(3, inv2), sj2));
    }

    cout << add(range1, range2) << endl;
    return 0;
}
