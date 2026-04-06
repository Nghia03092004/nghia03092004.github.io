#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    const ll N = 100000000LL;

    // Part 1: sum_{d=1}^{N} d * floor(N/d) via hyperbola method
    ll part1 = 0;
    for (ll d = 1; d <= N; ) {
        ll q = N / d;
        ll d_max = N / q;
        part1 += q * (d + d_max) * (d_max - d + 1) / 2;
        d = d_max + 1;
    }

    // Part 2: Gaussian integer contributions
    ll part2 = 0;
    for (ll b = 1; b * b + 1 <= N; b++) {
        for (ll a = 1; a * a + b * b <= N; a++) {
            if (__gcd(a, b) != 1) continue;
            ll norm = a * a + b * b;
            for (ll k = 1; k * norm <= N; k++) {
                ll cnt = N / (k * norm);
                part2 += 2 * k * a * cnt;
            }
        }
    }

    printf("%lld\n", part1 + part2);
    return 0;
}
