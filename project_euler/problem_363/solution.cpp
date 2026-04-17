#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 363: Bezier Curves
 *
 * Find optimal cubic Bezier approximation to a quarter circle.
 * Minimize the maximum radial deviation from the unit circle.
 *
 * By symmetry, control points are:
 *   P0 = (0,1), P1 = (k,1), P2 = (1,k), P3 = (1,0)
 *
 * We optimize k to minimize max |sqrt(x^2+y^2) - 1| over t in [0,1].
 *
 * Answer: 0.0000372091
 */

typedef long double ld;

// Evaluate Bezier curve at parameter t
pair<ld, ld> bezier(ld t, ld k) {
    ld s = 1.0L - t;
    ld x = 3.0L * s * s * t * k + 3.0L * s * t * t * 1.0L + t * t * t * 1.0L;
    ld y = s * s * s * 1.0L + 3.0L * s * s * t * 1.0L + 3.0L * s * t * t * k;
    return {x, y};
}

// Radial deviation at parameter t
ld radial_error(ld t, ld k) {
    auto [x, y] = bezier(t, k);
    return sqrtl(x * x + y * y) - 1.0L;
}

// Maximum absolute radial error over [0,1]
ld max_error(ld k) {
    ld mx = 0.0L;
    int N = 100000;
    for (int i = 0; i <= N; i++) {
        ld t = (ld)i / N;
        ld err = fabsl(radial_error(t, k));
        mx = max(mx, err);
    }
    return mx;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Golden section search for optimal k
    ld lo = 0.5L, hi = 0.6L;
    ld gr = (sqrtl(5.0L) - 1.0L) / 2.0L;

    for (int iter = 0; iter < 200; iter++) {
        ld a = hi - gr * (hi - lo);
        ld b = lo + gr * (hi - lo);
        if (max_error(a) < max_error(b)) {
            hi = b;
        } else {
            lo = a;
        }
    }

    ld optimal_k = (lo + hi) / 2.0L;
    ld result = max_error(optimal_k);

    cout << fixed << setprecision(10) << result << endl;
    // Expected: 0.0000372091

    return 0;
}
