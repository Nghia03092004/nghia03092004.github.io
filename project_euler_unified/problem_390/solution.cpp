#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * Project Euler Problem 390: Triangles with Non-Rational Sides and Integral Area
 *
 * Triangle sides: sqrt(1+b^2), sqrt(1+c^2), sqrt(b^2+c^2) for positive integers b,c.
 * Area = sqrt(b^2*c^2 + b^2 + c^2) / 2.
 * For integral area, both b,c must be even (b=2B, c=2C), and
 * Area = sqrt(4B^2*C^2 + B^2 + C^2), which must be a perfect integer.
 *
 * Equivalently, (b^2+1)(c^2+1) = (2*Area)^2 + 1.
 *
 * Approach: For each even a (smaller side param), solve the generalized Pell equation
 *   v^2 - (a^2+1)*t^2 = -a^2
 * The fundamental Pell solution for x^2-(a^2+1)y^2=1 is (2a^2+1, 2a).
 * Orbit representatives have even t0 in [2, a].
 * For each non-trivial rep (t0 < a), both +v0 and -v0 seeds generate distinct orbits.
 * The trivial rep t0=a yields only one orbit (v0=a^2).
 *
 * Given (v, t): b = a*t + v, n = a*b + t = 2*Area.
 * Constraint: n <= 2 * LIMIT.
 *
 * Complexity: O(sqrt(N) * sqrt(sqrt(N))) orbit reps + O(log N) per orbit chain.
 *
 * Answer: S(10^10) = 2919133642971
 */

int main() {
    const ll LIMIT = 10000000000LL;
    const ll N = 2 * LIMIT;
    ll ans = 0;
    ll cnt = 0;

    ll a_max = (ll)sqrtl((long double)N);
    while (a_max * a_max > N) a_max--;
    while ((a_max + 1) * (a_max + 1) <= N) a_max++;

    for (ll a = 2; a <= a_max; a += 2) {
        ll D = a * a + 1;
        ll a2 = a * a;
        ll p = 2 * a2 + 1;  // Pell fundamental x
        ll q = 2 * a;        // Pell fundamental y

        for (ll t0 = 2; t0 <= a; t0 += 2) {
            lll v2 = (lll)D * t0 * t0 - a2;
            if (v2 <= 0) continue;
            ll v0 = (ll)sqrtl((long double)v2);
            while ((lll)v0 * v0 < v2) v0++;
            while ((lll)v0 * v0 > v2) v0--;
            if ((lll)v0 * v0 != v2) continue;

            // Iterate orbit(s)
            int num_signs = (t0 < a) ? 2 : 1;
            for (int si = 0; si < num_signs; si++) {
                lll v_cur = (si == 0) ? v0 : -v0;
                lll t_cur = t0;

                while (true) {
                    if (v_cur > 0 && t_cur > 0) {
                        lll b = (lll)a * t_cur + v_cur;
                        lll nn = (lll)a * b + t_cur;
                        if (nn <= N) {
                            ans += (ll)(nn / 2);
                            cnt++;
                        } else {
                            break;
                        }
                    }
                    lll v_next = v_cur * p + (lll)D * t_cur * q;
                    lll t_next = v_cur * q + t_cur * p;
                    if (t_next > (lll)N * 100) break; // safety
                    v_cur = v_next;
                    t_cur = t_next;
                }
            }
        }
    }

    printf("S(10^10) = %lld\n", ans);
    printf("Count    = %lld\n", cnt);
    return 0;
}
