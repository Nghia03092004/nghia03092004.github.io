#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 155: Count distinct capacitance values using up to 18 unit capacitors.
 *
 * S(1) = {1}
 * S(n) = union over k=1..n/2 of { a+b, ab/(a+b) : a in S(k), b in S(n-k) }
 * C(n) = |S(1) union ... union S(n)|
 */

typedef pair<int, int> Frac; // (numerator, denominator), always reduced, den > 0

Frac make_frac(long long p, long long q) {
    if (q < 0) { p = -p; q = -q; }
    long long g = __gcd(abs(p), q);
    return {(int)(p / g), (int)(q / g)};
}

int main() {
    const int MAXN = 18;

    vector<set<Frac>> S(MAXN + 1);
    S[1].insert({1, 1});

    set<Frac> all_values;
    all_values.insert({1, 1});

    for (int n = 2; n <= MAXN; n++) {
        for (int k = 1; k * 2 <= n; k++) {
            int m = n - k;
            for (auto& a : S[k]) {
                for (auto& b : S[m]) {
                    // Parallel: a + b = (a.p*b.q + b.p*a.q) / (a.q*b.q)
                    long long pn = (long long)a.first * b.second + (long long)b.first * a.second;
                    long long pd = (long long)a.second * b.second;
                    S[n].insert(make_frac(pn, pd));

                    // Series: ab/(a+b) = (a.p*b.p) / (a.p*b.q + b.p*a.q)
                    long long sn = (long long)a.first * b.first;
                    long long sd = (long long)a.first * b.second + (long long)b.first * a.second;
                    S[n].insert(make_frac(sn, sd));
                }
            }
        }
        for (auto& f : S[n]) {
            all_values.insert(f);
        }
    }

    printf("%d\n", (int)all_values.size());
    return 0;
}
