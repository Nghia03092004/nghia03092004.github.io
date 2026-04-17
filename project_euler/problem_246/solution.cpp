#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 246: Tangents to an Ellipse
    // Count lattice points P outside ellipse where angle RPS > 45 degrees
    // Ellipse: center (3000,1500), a=7500, b^2=31250000

    const long long A2 = 56250000LL;
    const long long B2 = 31250000LL;
    const long long S = A2 + B2;
    const long long A2B2 = A2 * B2;
    const int cx = 3000, cy = 1500;

    // f(X,Y) = (X^2+Y^2-S)^2 - 4*(A^2*Y^2+B^2*X^2-A^2*B^2)
    // angle > 45 when: outside ellipse AND (inside director circle OR f < 0)
    // Equivalently: X^2*B2+Y^2*A2 > A2*B2 AND (X^2+Y^2 <= S OR f < 0)

    // For boundary detection, use exact integer arithmetic with __int128.
    // For each Y, find the max |X| where the condition holds by binary search.

    auto isInside = [&](long long X, long long Y2) -> bool {
        // Check: outside ellipse AND (inside director circle OR f < 0)
        long long X2 = X * X;
        __int128 ev = (__int128)X2 * B2 + (__int128)Y2 * A2;
        if (ev <= (__int128)A2B2) return false; // inside or on ellipse

        long long r2 = X2 + Y2;
        if (r2 <= S) return true; // inside director circle

        // Check f < 0
        __int128 diff = r2 - S;
        __int128 lhs = diff * diff;
        __int128 rhs = 4 * ((__int128)A2 * Y2 + (__int128)B2 * X2 - (__int128)A2B2);
        return lhs < rhs;
    };

    long long count = 0;

    // Y range: b ~ 5590, isoptic extends to ~18950 from center
    for (int y = -17500; y <= 20500; y++) {
        long long Y = y - cy;
        long long Y2 = Y * Y;

        // Get approximate iso_hi using floating point
        double p_val = 2.0*Y2 - 2.0*A2 - 6.0*B2;
        double Y4 = (double)Y2 * Y2;
        double q_val = Y4 - (6.0*A2+2.0*B2)*Y2 + (double)A2*A2 + 6.0*A2*B2 + (double)B2*B2;
        double disc = p_val*p_val - 4*q_val;
        if (disc < 0) continue;
        double iso_hi_sq = (-p_val + sqrt(disc)) / 2.0;
        if (iso_hi_sq <= 0) continue;
        double iso_hi_approx = sqrt(iso_hi_sq);

        // Get approximate ell_lo
        long long ell_rhs_num = A2 * (B2 - Y2);
        double ell_lo_approx;
        if (ell_rhs_num <= 0) {
            ell_lo_approx = 0;
        } else {
            ell_lo_approx = sqrt((double)ell_rhs_num / B2);
        }

        if (ell_lo_approx >= iso_hi_approx + 2) continue;

        // Positive X side: find exact range using exact checks
        // Start from approximate boundaries and adjust
        int x_lo = (int)ceil(cx + ell_lo_approx);
        int x_hi = (int)floor(cx + iso_hi_approx) + 1; // +1 for safety

        // Adjust x_lo: find first x where isInside
        // x_lo should be the smallest x >= cx + ell_lo_approx that's inside
        while (x_lo <= x_hi && !isInside(x_lo - cx, Y2)) x_lo++;

        // Adjust x_hi: find last x where isInside
        // Start from approximate and search
        while (x_hi >= x_lo && !isInside(x_hi - cx, Y2)) x_hi--;

        // Verify we haven't missed points above x_hi
        // (unlikely given the +1 safety margin, but check)
        while (x_hi + 1 <= cx + (int)iso_hi_approx + 3 && isInside(x_hi + 1 - cx, Y2)) x_hi++;

        if (x_lo <= x_hi)
            count += x_hi - x_lo + 1;

        // Negative X side (use cx-1 as upper bound when ell_lo=0 to avoid
        // double-counting the x=cx column already covered by positive side)
        int x_lo2 = (int)ceil(cx - iso_hi_approx) - 1; // -1 for safety
        int x_hi2 = (ell_lo_approx < 0.5) ? cx - 1 : (int)floor(cx - ell_lo_approx);

        while (x_lo2 <= x_hi2 && !isInside(x_lo2 - cx, Y2)) x_lo2++;
        while (x_hi2 >= x_lo2 && !isInside(x_hi2 - cx, Y2)) x_hi2--;
        while (x_lo2 - 1 >= cx - (int)iso_hi_approx - 3 && isInside(x_lo2 - 1 - cx, Y2)) x_lo2--;

        if (x_lo2 <= x_hi2)
            count += x_hi2 - x_lo2 + 1;
    }

    cout << count << endl;
    return 0;
}
