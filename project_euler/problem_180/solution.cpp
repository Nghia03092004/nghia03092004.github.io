#include <bits/stdc++.h>
using namespace std;

// Problem 180: Rational Zeros of a Function of Three Variables
// Find S(35) = sum of distinct s = x+y+z from golden triples.
// x, y, z are proper fractions a/b with 1 <= a < b <= 35.
// Equations: m=1 (x+y=z), m=-1 (z=xy/(x+y)), m=2 (z=sqrt(x^2+y^2)),
//            m=-2 (z=xy/sqrt(x^2+y^2)).

typedef long long ll;
typedef __int128 lll;

ll gcd_ll(ll a, ll b) {
    a = abs(a); b = abs(b);
    while (b) { a %= b; swap(a,b); }
    return a;
}

struct Frac {
    ll num, den; // always reduced, den > 0
    Frac(ll n = 0, ll d = 1) {
        if (d < 0) { n = -n; d = -d; }
        ll g = gcd_ll(abs(n), d);
        num = n / g; den = d / g;
    }
    bool operator<(const Frac& o) const {
        return (lll)num * o.den < (lll)o.num * den;
    }
    bool operator==(const Frac& o) const {
        return num == o.num && den == o.den;
    }
    bool operator<=(const Frac& o) const { return !(o < *this); }
    bool operator>(const Frac& o) const { return o < *this; }
    bool operator>=(const Frac& o) const { return !(*this < o); }
    Frac operator+(const Frac& o) const {
        return Frac(num * o.den + o.num * den, den * o.den);
    }
    Frac operator*(const Frac& o) const {
        return Frac(num * o.num, den * o.den);
    }
    Frac operator/(const Frac& o) const {
        return Frac(num * o.den, den * o.num);
    }
};

int main() {
    const int K = 35;

    // Generate all proper fractions a/b with 1 <= a < b <= K
    vector<Frac> rats;
    set<pair<ll,ll>> rat_set;
    for (int b = 2; b <= K; b++) {
        for (int a = 1; a < b; a++) {
            Frac f(a, b);
            if (rat_set.insert({f.num, f.den}).second) {
                rats.push_back(f);
            }
        }
    }

    auto is_valid = [&](const Frac& z) -> bool {
        if (z.num <= 0 || z.den <= 0) return false;
        if (z.num >= z.den) return false; // z >= 1
        return z.den <= K;
    };

    auto isqrt_ll = [](ll n) -> ll {
        if (n < 0) return -1;
        ll s = (ll)sqrt((double)n);
        while (s * s > n) s--;
        while ((s+1)*(s+1) <= n) s++;
        return s;
    };

    set<pair<ll,ll>> s_values;

    for (auto& x : rats) {
        for (auto& y : rats) {
            // m=1: z = x+y
            {
                Frac z = x + y;
                if (is_valid(z)) {
                    Frac s = x + y + z;
                    s_values.insert({s.num, s.den});
                }
            }

            // m=-1: z = xy/(x+y)
            {
                Frac xy = x * y;
                Frac xpy = x + y;
                Frac z = xy / xpy;
                if (is_valid(z)) {
                    Frac s = x + y + z;
                    s_values.insert({s.num, s.den});
                }
            }

            // m=2: z = sqrt(x^2+y^2)
            // x^2+y^2 = (x.num^2*y.den^2 + y.num^2*x.den^2) / (x.den^2*y.den^2)
            {
                ll num2 = x.num*x.num*y.den*y.den + y.num*y.num*x.den*x.den;
                ll den2 = x.den*x.den*y.den*y.den;
                ll g = gcd_ll(num2, den2);
                num2 /= g; den2 /= g;
                ll sn = isqrt_ll(num2);
                ll sd = isqrt_ll(den2);
                if (sn >= 0 && sd > 0 && sn*sn == num2 && sd*sd == den2) {
                    Frac z(sn, sd);
                    if (is_valid(z)) {
                        Frac s = x + y + z;
                        s_values.insert({s.num, s.den});
                    }

                    // m=-2: z = xy/sqrt(x^2+y^2)
                    Frac sqrt_sum(sn, sd);
                    Frac xy = x * y;
                    Frac z2 = xy / sqrt_sum;
                    if (is_valid(z2)) {
                        Frac s = x + y + z2;
                        s_values.insert({s.num, s.den});
                    }
                }
            }
        }
    }

    // Sum all distinct s values as a single fraction
    // Use __int128 to avoid overflow
    lll total_num = 0, total_den = 1;
    for (auto& [n, d] : s_values) {
        // total_num/total_den + n/d = (total_num*d + n*total_den) / (total_den*d)
        total_num = total_num * d + (lll)n * total_den;
        total_den = total_den * d;
        // Reduce periodically to prevent overflow
        lll g = 0;
        { lll a = total_num < 0 ? -total_num : total_num, b = total_den;
          while (b) { a %= b; swap(a,b); } g = a; }
        total_num /= g;
        total_den /= g;
    }

    if (total_den < 0) { total_num = -total_num; total_den = -total_den; }

    // Print total_num + total_den
    lll result = total_num + total_den;

    // Print __int128
    auto print128 = [](lll x) {
        if (x < 0) { putchar('-'); x = -x; }
        if (x == 0) { putchar('0'); return; }
        string s;
        while (x > 0) { s += '0' + (int)(x % 10); x /= 10; }
        reverse(s.begin(), s.end());
        printf("%s", s.c_str());
    };

    print128(result);
    putchar('\n');

    return 0;
}
