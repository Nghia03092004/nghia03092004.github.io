#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Problem 198: Ambiguous Numbers
//
// x = p/q is ambiguous iff it equals (ad+bc)/(2bd) for Farey neighbors a/b, c/d
// with bc-ad=1. The denominator is always 2bd in lowest terms.
// So we need bd <= N/2 and the midpoint in (0, 1/100).
//
// Split into: b=1 case, small b (100..B), and large b (B+1..N/2) via d-iteration.

ll mod_inverse(ll a, ll m) {
    // Extended Euclidean
    ll g = __gcd(a, m);
    if (g != 1) return -1;
    // Using pow with modular inverse for prime... but m may not be prime.
    // Use extended gcd.
    ll x = 1, y = 0, x1 = 0, y1 = 1, a0 = a, m0 = m;
    while (m0 != 0) {
        ll q = a0 / m0;
        ll tmp;
        tmp = x - q * x1; x = x1; x1 = tmp;
        tmp = y - q * y1; y = y1; y1 = tmp;
        tmp = a0 - q * m0; a0 = m0; m0 = tmp;
    }
    return ((x % m) + m) % m;
}

int main() {
    ios_base::sync_with_stdio(false);

    const ll N = 100000000LL;
    const ll HALF = N / 2;
    const ll B = (ll)sqrt((double)HALF);

    ll count = 0;

    // b = 1: d > 50, d <= HALF
    count += HALF - 50;

    // Small b: 100 to B
    for (ll b = 100; b <= B; b++) {
        ll d_max = HALF / b;
        if (d_max < 1) break;

        ll a_upper;
        if (b % 100 == 0)
            a_upper = b / 100 - 1;
        else
            a_upper = b / 100;

        for (ll a = 1; a <= a_upper; a++) {
            if (__gcd(a, b) != 1) continue;

            ll a_inv = mod_inverse(a, b);
            ll d_res = (b - a_inv) % b;  // (-a_inv) mod b
            if (d_res <= 0) d_res += b;

            ll gap = b - 100 * a;
            if (gap <= 0) continue;

            ll d_min = 50 / gap + 1; // strict >

            ll first_d;
            if (d_res >= d_min)
                first_d = d_res;
            else {
                ll k = (d_min - d_res + b - 1) / b;
                first_d = d_res + k * b;
            }

            if (first_d > d_max) continue;
            count += (d_max - first_d) / b + 1;
        }
    }

    // Large b: iterate d from 1 to B, then m values
    for (ll d = 1; d <= B; d++) {
        ll b_max = HALF / d;
        if (b_max <= B) continue;

        ll m_min;
        if ((99 * d) % 100 == 0)
            m_min = 99 * d / 100;
        else
            m_min = 99 * d / 100 + 1;

        ll m_max = d - 1;

        for (ll m = m_min; m <= m_max; m++) {
            if (__gcd(m, d) != 1) continue;

            ll m_inv = mod_inverse(m, d);
            ll b_res = (d - m_inv) % d; // (-m^{-1}) mod d
            if (b_res <= 0) b_res += d;

            ll first_b;
            if (b_res >= B + 1)
                first_b = b_res;
            else {
                ll k = (B + 1 - b_res + d - 1) / d;
                first_b = b_res + k * d;
            }

            if (first_b > b_max) continue;
            count += (b_max - first_b) / d + 1;
        }
    }

    cout << count << endl;
    return 0;
}
