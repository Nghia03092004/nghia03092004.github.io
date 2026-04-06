#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 730: Shifted Pythagorean Triples
 *
 * p^2 + q^2 + k = r^2, gcd(p,q,r)=1, p<=q<=r, p+q+r<=n.
 * For each p: r^2 - q^2 = p^2+k, factor p^2+k = d1*d2, d1<=d2, same parity.
 * q = (d2-d1)/2, r = (d1+d2)/2.
 */

vector<int> get_divisors(long long n) {
    vector<int> divs;
    for (long long d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            divs.push_back(d);
            if (d != n / d) divs.push_back(n / d);
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

long long P_k(int k, int n) {
    long long count = 0;
    for (int p = 1; 3*p <= n; p++) {
        long long val = (long long)p*p + k;
        auto divs = get_divisors(val);
        for (int d1 : divs) {
            long long d2 = val / d1;
            if (d1 > d2) break;
            if ((d1 & 1) != (d2 & 1)) continue;
            long long q = (d2 - d1) / 2;
            long long r = (d1 + d2) / 2;
            if (q < p) continue;
            if (p + q + r > n) continue;
            if (__gcd(__gcd((long long)p, q), r) != 1) continue;
            count++;
        }
    }
    return count;
}

int main() {
    printf("P_0(10000) = %lld\n", P_k(0, 10000));
    printf("P_20(10000) = %lld\n", P_k(20, 10000));

    long long total = 0;
    for (int k = 0; k <= 10; k++) total += P_k(k, 10000);
    printf("S(10, 10000) = %lld\n", total);

    return 0;
}
