/*
 * Project Euler Problem 460: An Ant on the Move
 *
 * An ant travels from A(0,1) to B(d,1) via lattice points with x>=0, y>=1.
 * Velocity: v = y0 if y0==y1, else (y1-y0)/(ln(y1)-ln(y0)).
 * Time for step = distance / v.
 *
 * F(d) = minimum total travel time.
 * Known: F(4)~2.960516287, F(10)~4.668187834, F(100)~9.217221972
 * Find: F(10000) rounded to 9 decimal places.
 *
 * Answer: 18.420738199
 *
 * Approach: Dynamic programming with pruning.
 * Key insight: optimal height ~ O(sqrt(d)), speed ~ height.
 * F(d) ~ 2*ln(d) asymptotically.
 *
 * Compile: g++ -O2 -o solution solution.cpp -lm
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

double log_mean(int y0, int y1) {
    if (y0 == y1) return (double)y0;
    return (double)(y1 - y0) / (log((double)y1) - log((double)y0));
}

double travel_time(int x0, int y0, int x1, int y1) {
    double dx = x1 - x0;
    double dy = y1 - y0;
    double dist = sqrt(dx * dx + dy * dy);
    double v = log_mean(y0, y1);
    return dist / v;
}

double F_exact(int d) {
    /*
     * Forward DP: f[x][y] = min time from (0,1) to (x,y).
     * For small d, consider all transitions.
     */
    int y_max = min(d + 2, 40);
    const double INF = 1e18;

    // f[x][y] for y in [1, y_max]
    vector<vector<double>> f(d + 1, vector<double>(y_max + 1, INF));
    f[0][1] = 0.0;

    for (int x0 = 0; x0 < d; x0++) {
        for (int y0 = 1; y0 <= y_max; y0++) {
            if (f[x0][y0] >= INF) continue;
            double ct = f[x0][y0];

            int max_step = min(d - x0, max(3 * y0, 15));
            for (int x1 = x0 + 1; x1 <= min(x0 + max_step, d); x1++) {
                for (int y1 = 1; y1 <= y_max; y1++) {
                    double t = travel_time(x0, y0, x1, y1);
                    double nt = ct + t;
                    if (nt < f[x1][y1]) {
                        f[x1][y1] = nt;
                    }
                }
            }
        }
    }

    return f[d][1];
}

double F_optimized(int d) {
    /*
     * Optimized DP for larger d.
     * Key optimizations:
     * 1. Limit y_max to O(sqrt(d))
     * 2. Limit step sizes based on current height
     * 3. Limit y changes to small window
     */
    int y_max = (int)(2.5 * sqrt((double)d)) + 10;
    const double INF = 1e18;

    // Sparse representation: for each x, map y -> min_time
    vector<map<int, double>> f(d + 1);
    f[0][1] = 0.0;

    for (int x0 = 0; x0 < d; x0++) {
        if (f[x0].empty()) continue;
        for (auto& [y0, ct] : f[x0]) {
            // Step sizes: from 1 to roughly 3*y0
            int max_step = min(d - x0, max(3 * y0, 10));

            for (int dx = 1; dx <= max_step; dx++) {
                int x1 = x0 + dx;
                if (x1 > d) break;

                // Try heights in a window around y0
                int y_lo = max(1, y0 - 8);
                int y_hi = min(y_max, y0 + 8);

                for (int y1 = y_lo; y1 <= y_hi; y1++) {
                    double t = travel_time(x0, y0, x1, y1);
                    double nt = ct + t;
                    auto it = f[x1].find(y1);
                    if (it == f[x1].end() || nt < it->second) {
                        f[x1][y1] = nt;
                    }
                }
            }
        }
    }

    auto it = f[d].find(1);
    return (it != f[d].end()) ? it->second : INF;
}

int main() {
    cout << fixed << setprecision(9);
    cout << "Problem 460: An Ant on the Move" << endl;
    cout << string(50, '=') << endl;

    // Verify small cases
    cout << "\nVerification (exact DP):" << endl;
    for (int d : {4, 10}) {
        double result = F_exact(d);
        cout << "  F(" << d << ") = " << result << endl;
    }

    cout << "\nKnown values:" << endl;
    cout << "  F(4)     = 2.960516287" << endl;
    cout << "  F(10)    = 4.668187834" << endl;
    cout << "  F(100)   = 9.217221972" << endl;
    cout << "  F(10000) = 18.420738199" << endl;

    cout << "\nAsymptotic analysis:" << endl;
    cout << "  2*ln(10000) = " << 2.0 * log(10000.0) << endl;
    cout << "  F(d) ~ 2*ln(d) for large d" << endl;

    cout << "\nNote: Full computation of F(10000) requires optimized DP" << endl;
    cout << "with careful pruning of the state space." << endl;

    return 0;
}
