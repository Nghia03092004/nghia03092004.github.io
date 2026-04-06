#include <bits/stdc++.h>
using namespace std;

// s(r) = sum_{k=1}^{5000} (900 - 3k) * r^{k-1}
// Using closed form:
// s(r) = 900 * (r^5000 - 1)/(r - 1) - 3 * (5000*r^5001 - 5001*r^5000 + 1) / (r-1)^2

// For numerical stability, use direct summation with long double
// or use the closed form carefully.

long double s_func(long double r) {
    // Direct summation might lose precision for r near 1 with 5000 terms.
    // Use closed form with long double.
    long double r5000 = powl(r, 5000);
    long double r5001 = r5000 * r;
    long double rm1 = r - 1.0L;

    long double geo = (r5000 - 1.0L) / rm1;
    long double ageo = (5000.0L * r5001 - 5001.0L * r5000 + 1.0L) / (rm1 * rm1);

    return 900.0L * geo - 3.0L * ageo;
}

int main() {
    long double target = -600000000000.0L;

    // Bisection on [1.0, 1.01]
    long double lo = 1.0L, hi = 1.01L;

    // Verify signs
    // s(1.0) is about -33007500, so s(1.0) - target > 0
    // s(1.01) should be very negative, so s(1.01) - target < 0

    for (int iter = 0; iter < 200; iter++) {
        long double mid = (lo + hi) / 2.0L;
        long double val = s_func(mid);
        if (val > target) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    printf("%.12Lf\n", lo);
    return 0;
}
