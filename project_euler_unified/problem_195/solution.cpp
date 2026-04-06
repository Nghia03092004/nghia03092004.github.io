#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 195: Inscribed Circles of Triangles with One Angle of 60 Degrees
 *
 * Count integer-sided triangles with one 60-degree angle whose incircle
 * radius r <= R = 10^5.
 *
 * Parameterization: for p > q > 0, gcd(p,q) = 1:
 *   a0 = p^2 + 2pq, b0 = 2pq + q^2, c0 = p^2 + pq + q^2
 *   If (p-q) % 3 == 0: divide all by 3
 *   r0 = sqrt(3)*a0*b0/(2*(a0+b0+c0))
 *   count += floor(R / r0) for each primitive triple
 *
 * Optimization: iterate q in outer loop (1, 2, ...) and p in inner loop.
 * For fixed q, as p grows, r0 grows roughly as sqrt(3)*p*q/(2*(something)).
 * We break when r0 > R for the minimum possible case.
 *
 * Actually, the key insight: r0 = sqrt(3)*a0*b0/(2*s) where s=a0+b0+c0.
 * For large p: a0 ~ p^2, b0 ~ 2pq, c0 ~ p^2, s ~ 2p^2+2pq
 * r0 ~ sqrt(3)*2p^3*q/(2*(2p^2+2pq)) = sqrt(3)*p^2*q/(2(p+q))
 * For large p >> q: r0 ~ sqrt(3)*p*q/2
 * So p <= 2R/(sqrt(3)*q) roughly.
 *
 * Answer: 75085391
 */

int main() {
    const double R = 100000.0;
    const double sqrt3 = sqrt(3.0);
    ll count = 0;

    // Outer loop: q from 1 to some max
    // For q, p ranges from q+1 up to about 2R/(sqrt3*q) + margin
    // q_max: when p = q+1 (minimum p), r0 is minimized. But even for
    // large q, with p = q+1, r0 ~ sqrt(3)*(q+1)*q/2 which must be <= R
    // So q <= about 2R/sqrt(3) ~ 115470 at most, but for large q we need
    // p very close to q, which gives small r0.
    // Actually for p = q+1, a0 = (q+1)^2 + 2(q+1)q = 3q^2+4q+1, b0 = 2(q+1)q+q^2 = 3q^2+2q
    // c0 = (q+1)^2+(q+1)q+q^2 = 3q^2+3q+1
    // sum = 9q^2+9q+2, r0 = sqrt(3)*(3q^2+4q+1)*(3q^2+2q)/(2*(9q^2+9q+2))
    // For large q: r0 ~ sqrt(3)*9q^4/(2*9q^2) = sqrt(3)*q^2/2
    // So q <= sqrt(2R/sqrt3) ~ 340

    int q_max = (int)sqrt(2.0 * R / sqrt3) + 100;

    for (int q = 1; q <= q_max; q++) {
        int p_max = (int)(2.0 * R / (sqrt3 * q)) + 100;
        for (int p = q + 1; p <= p_max; p++) {
            if (__gcd(p, q) != 1) continue;

            ll a0 = (ll)p * p + 2LL * p * q;
            ll b0 = 2LL * p * q + (ll)q * q;
            ll c0 = (ll)p * p + (ll)p * q + (ll)q * q;

            if ((p - q) % 3 == 0) {
                a0 /= 3;
                b0 /= 3;
                c0 /= 3;
            }

            double r0 = sqrt3 * (double)a0 * (double)b0 / (2.0 * (double)(a0 + b0 + c0));

            if (r0 > R) break;  // p increasing => r0 increasing for fixed q

            ll k = (ll)(R / r0);
            if (k >= 1) count += k;
        }
    }

    cout << count << endl;
    return 0;
}
