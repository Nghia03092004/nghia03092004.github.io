#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;
typedef unsigned long long ull;

// Integer square root
ll isqrt(ll n) {
    ll x = (ll)sqrt((double)n);
    while (x * x > n) x--;
    while ((x+1)*(x+1) <= n) x++;
    return x;
}

ll best_denominator(ll d, ll Q) {
    ll a0 = isqrt(d);
    if (a0 * a0 == d) return 0; // perfect square, skip

    // Continued fraction expansion of sqrt(d)
    // We track convergents h2/k2 (two steps back), h1/k1 (one step back)
    ll m = 0, dd = 1, a = a0;
    ll h2 = 0, h1 = 1, k2 = 1, k1 = 0;

    // Process a0
    ll h = a0 * h1 + h2;
    ll k = a0 * k1 + k2;
    h2 = h1; h1 = h;
    k2 = k1; k1 = k;

    while (true) {
        m = a * dd - m;
        dd = (d - m * m) / dd;
        a = (a0 + m) / dd;

        ll knext = a * k1 + k2;
        if (knext > Q) {
            // The full convergent exceeds Q.
            // Check semiconvergent with max valid m_sc
            ll m_sc = (Q - k2) / k1;
            if (m_sc < 1) return k1; // no valid semiconvergent

            ll qs = m_sc * k1 + k2;
            ll ps = m_sc * h1 + h2;

            // Compare |qs*sqrt(d) - ps| vs |k1*sqrt(d) - h1|
            // |qs*sqrt(d) - ps|^2 = qs^2*d - 2*ps*qs*sqrt(d) + ps^2
            // But we can use: for convergent p_{n-1}/q_{n-1}, the error is
            // 1/(q_{n-1}*(a_n*q_{n-1}+q_{n-2})) approximately.
            // More precisely:
            // |q*sqrt(d)-p| compared by squaring is tricky due to sqrt.
            //
            // Instead use: semiconvergent is better iff
            //   |qs*sqrt(d) - ps| < |k1*sqrt(d) - h1|
            // Both sides positive (after abs). Square both sides:
            //   (qs^2*d + ps^2 - 2*ps*qs*sqrt(d)) < (k1^2*d + h1^2 - 2*h1*k1*sqrt(d))
            // Let A = qs^2*d + ps^2 - k1^2*d - h1^2
            // Let B = 2*(ps*qs - h1*k1)
            // We need A < B*sqrt(d)
            // If B > 0: A < B*sqrt(d) iff A < 0 or A^2 < B^2*d
            // If B <= 0: A < B*sqrt(d) iff A < 0 and A^2 > B^2*d

            lll A = (lll)qs*qs*d + (lll)ps*ps - (lll)k1*k1*d - (lll)h1*h1;
            lll B = 2*((lll)ps*qs - (lll)h1*k1);

            bool semi_better;
            if (B > 0) {
                if (A <= 0) semi_better = true;
                else semi_better = (A * A < B * B * d);
            } else {
                if (A >= 0) semi_better = false;
                else semi_better = (A * A > B * B * d);
            }

            return semi_better ? qs : k1;
        }

        ll hnext = a * h1 + h2;
        h2 = h1; h1 = hnext;
        k2 = k1; k1 = knext;

        if (a == 2 * a0 && k1 <= Q) {
            // Completed one period; if still within bound, continue
            // (the CF is periodic, it will repeat)
        }
    }
}

int main() {
    const ll Q = 1000000000000LL; // 10^12
    const int N = 100000;
    ll total = 0;
    for (int d = 2; d <= N; d++) {
        ll sq = isqrt(d);
        if (sq * sq == d) continue;
        total += best_denominator(d, Q);
    }
    cout << total << endl;
    return 0;
}
