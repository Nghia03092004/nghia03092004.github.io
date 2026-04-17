#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * Problem 264: Triangle Centres
 *
 * Find all triangles ABC with:
 * - Integer coordinate vertices on circle x^2+y^2 = n (circumcenter at origin)
 * - Orthocenter at H(5,0): A+B+C = (5,0)
 * - Perimeter <= 100000
 * - Non-degenerate (positive area)
 *
 * Sum of all such perimeters, rounded to 4 decimal places.
 *
 * Key insight: for fixed A = (a1,a2) on x^2+y^2=n, the constraint
 * C = (5-a1-b1, -a2-b2) on the same circle gives:
 *   (2*a1-10)*b1 + 2*a2*b2 = 10*a1 - 25 - n
 * Combined with b1^2+b2^2 = n, this is a line-circle intersection
 * giving at most 2 solutions for B.
 *
 * The discriminant simplifies to D = n*(L^2+4*a2^2) - R^2 where
 * L = 2*a1-10, R = 10*a1-25-n. D must be a non-negative perfect square.
 *
 * Each unordered triangle counted 6 times (3! orderings).
 * Total: 155 triangles, sum of perimeters = 2816417.1055.
 *
 * Answer: 2816417.1055
 */

int main() {
    const double MAX_PERIMETER = 100000.0;
    const ll R_MAX = 20000LL;  // Sufficient: no triangles found beyond R=20000

    double total_perimeter = 0.0;
    ll triangle_count = 0;

    for (ll a1 = -R_MAX; a1 <= R_MAX; a1++) {
        ll L = 2*a1 - 10;
        ll max_a2_sq = R_MAX * R_MAX - a1 * a1;
        if (max_a2_sq < 0) continue;
        ll max_a2 = (ll)sqrtl((long double)max_a2_sq);
        while ((max_a2+1)*(max_a2+1) <= max_a2_sq) max_a2++;

        for (ll a2 = 1; a2 <= max_a2; a2++) {
            ll n = a1*a1 + a2*a2;
            ll R_val = 10*a1 - 25 - n;

            // D = n*(L^2+4*a2^2) - R_val^2. Must be a perfect square >= 0.
            lll D = (lll)n * ((lll)L*L + 4*(lll)a2*a2) - (lll)R_val * R_val;
            if (D < 0) continue;

            ll sq_D = (ll)sqrtl((long double)D);
            while ((lll)sq_D * sq_D < D) sq_D++;
            while ((lll)sq_D * sq_D > D) sq_D--;
            if ((lll)sq_D * sq_D != D) continue;

            // disc = 16*a2^2*D, sqrt(disc) = 4*a2*sq_D
            for (int a2_sign = -1; a2_sign <= 1; a2_sign += 2) {
                ll a2v = a2 * a2_sign;
                ll M = 2 * a2v;

                lll A_c = (lll)M*M + (lll)L*L;
                lll B_c = -2*(lll)R_val*(lll)L;
                ll sq_disc = 4 * a2 * sq_D;

                for (int sign = -1; sign <= 1; sign += 2) {
                    lll num = -B_c + sign * sq_disc;
                    lll denom = 2 * A_c;
                    if (denom == 0) continue;
                    if (num % denom != 0) continue;
                    ll b1 = (ll)(num / denom);

                    lll num2 = (lll)R_val - (lll)L * b1;
                    if (num2 % M != 0) continue;
                    ll b2 = (ll)(num2 / M);

                    if (b1*b1 + b2*b2 != n) continue;

                    ll c1 = 5 - a1 - b1;
                    ll c2 = -a2v - b2;
                    if (c1*c1 + c2*c2 != n) continue;

                    ll cross = (b1-a1)*(c2-a2v) - (b2-a2v)*(c1-a1);
                    if (cross == 0) continue;

                    double dAB = sqrt((double)((a1-b1)*(a1-b1)+(a2v-b2)*(a2v-b2)));
                    double dBC = sqrt((double)((b1-c1)*(b1-c1)+(b2-c2)*(b2-c2)));
                    double dCA = sqrt((double)((c1-a1)*(c1-a1)+(c2-a2v)*(c2-a2v)));
                    double peri = dAB + dBC + dCA;

                    if (peri <= MAX_PERIMETER) {
                        total_perimeter += peri;
                        triangle_count++;
                    }
                }
            }
        }

        // Handle a2 = 0
        {
            ll n = a1*a1;
            if (n > 0) {
                ll R_val = 10*a1 - 25 - n;
                if (L == 0) {
                    // a1=5, n=25
                    for (ll bb1 = -5; bb1 <= 5; bb1++) {
                        ll bb2_sq = 25 - bb1*bb1;
                        if (bb2_sq < 0) continue;
                        ll bb2 = (ll)sqrt((double)bb2_sq);
                        if (bb2*bb2 != bb2_sq) continue;
                        for (int s = (bb2==0?1:-1); s <= 1; s += 2) {
                            ll b2v = s*bb2;
                            ll c1v = -bb1, c2v = -b2v;
                            if (c1v*c1v+c2v*c2v != 25) continue;
                            ll cross = (bb1-5)*c2v - b2v*(c1v-5);
                            if (cross == 0) continue;
                            double d1 = sqrt((double)((5-bb1)*(5-bb1)+b2v*b2v));
                            double d2 = sqrt((double)((bb1-c1v)*(bb1-c1v)+(b2v-c2v)*(b2v-c2v)));
                            double d3 = sqrt((double)((c1v-5)*(c1v-5)+c2v*c2v));
                            double peri = d1+d2+d3;
                            if (peri <= MAX_PERIMETER) {
                                total_perimeter += peri;
                                triangle_count++;
                            }
                        }
                    }
                } else if (R_val % L == 0) {
                    ll b1 = R_val / L;
                    ll b2_sq = n - b1*b1;
                    if (b2_sq >= 0) {
                        ll b2 = (ll)sqrt((double)b2_sq);
                        if (b2*b2 == b2_sq) {
                            for (int s = (b2==0?1:-1); s <= 1; s += 2) {
                                ll b2v = s*b2;
                                ll c1 = 5-a1-b1, c2 = -b2v;
                                if (c1*c1+c2*c2 != n) continue;
                                ll cross = (b1-a1)*c2 - b2v*(c1-a1);
                                if (cross == 0) continue;
                                double d1 = sqrt((double)((a1-b1)*(a1-b1)+b2v*b2v));
                                double d2 = sqrt((double)((b1-c1)*(b1-c1)+(b2v-c2)*(b2v-c2)));
                                double d3 = sqrt((double)((c1-a1)*(c1-a1)+c2*c2));
                                double peri = d1+d2+d3;
                                if (peri <= MAX_PERIMETER) {
                                    total_perimeter += peri;
                                    triangle_count++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    total_perimeter /= 6.0;
    triangle_count /= 6;
    printf("%.4f\n", total_perimeter);
    return 0;
}
