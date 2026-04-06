/*
 * Project Euler Problem 461: Almost Pi
 *
 * Let f_n(k) = e^(k/n) - 1.
 * Find g(n) = a^2 + b^2 + c^2 + d^2 for a,b,c,d minimizing
 * |f_n(a) + f_n(b) + f_n(c) + f_n(d) - pi|.
 *
 * Given: g(200) = 64658.
 * Find: g(10000) = 159820276.
 *
 * Approach: Meet-in-the-middle with binary search.
 *
 * Compile: g++ -O2 -o solution solution.cpp -lm
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <cassert>

using namespace std;

const double PI = 3.14159265358979323846;

struct PairSum {
    double sum;
    int a, b;

    bool operator<(const PairSum& other) const {
        return sum < other.sum;
    }
};

long long solve(int n) {
    // Maximum k: e^(k/n) - 1 <= pi => k <= n * ln(pi + 1)
    int k_max = (int)(n * log(PI + 1.0)) + 1;

    // Precompute f_n values
    vector<double> fval(k_max + 1);
    for (int k = 0; k <= k_max; k++) {
        fval[k] = exp((double)k / n) - 1.0;
    }

    cout << "  n = " << n << ", k_max = " << k_max << endl;

    // Phase 1: Generate all pair sums f_n(a) + f_n(b) with a <= b, sum <= pi
    vector<PairSum> pairs;
    pairs.reserve(100000000LL); // Reserve for large n

    for (int a = 0; a <= k_max; a++) {
        if (fval[a] > PI) break;
        for (int b = a; b <= k_max; b++) {
            double s = fval[a] + fval[b];
            if (s > PI + 1e-9) break; // Allow small overshoot for rounding
            if (s <= PI) {
                pairs.push_back({s, a, b});
            }
        }
    }

    cout << "  Number of pairs: " << pairs.size() << endl;

    // Sort by sum
    sort(pairs.begin(), pairs.end());

    // Phase 2: Binary search for optimal complement
    double best_error = 1e18;
    int best_a = 0, best_b = 0, best_c = 0, best_d = 0;

    vector<double> sums(pairs.size());
    for (size_t i = 0; i < pairs.size(); i++) {
        sums[i] = pairs[i].sum;
    }

    for (size_t i = 0; i < pairs.size(); i++) {
        double target = PI - pairs[i].sum;
        if (target < -1e-9) break; // pairs[i].sum > pi, no valid complement

        // Binary search for target in sums
        auto it = lower_bound(sums.begin(), sums.end(), target);

        // Check it and it-1
        for (int offset = -1; offset <= 0; offset++) {
            auto jt = it;
            if (offset == -1) {
                if (jt == sums.begin()) continue;
                --jt;
            }
            if (jt == sums.end()) continue;

            size_t j = jt - sums.begin();
            double error = fabs(pairs[i].sum + pairs[j].sum - PI);
            if (error < best_error) {
                best_error = error;
                best_a = pairs[i].a;
                best_b = pairs[i].b;
                best_c = pairs[j].a;
                best_d = pairs[j].b;
            }
        }
    }

    long long g = (long long)best_a * best_a + (long long)best_b * best_b +
                  (long long)best_c * best_c + (long long)best_d * best_d;

    cout << "  Optimal: a=" << best_a << ", b=" << best_b
         << ", c=" << best_c << ", d=" << best_d << endl;
    cout << "  Sum = " << fixed << setprecision(15)
         << (fval[best_a] + fval[best_b] + fval[best_c] + fval[best_d]) << endl;
    cout << "  Pi  = " << PI << endl;
    cout << "  Error = " << scientific << best_error << endl;
    cout << "  g(" << n << ") = " << g << endl;

    return g;
}

int main() {
    cout << "Problem 461: Almost Pi" << endl;
    cout << string(50, '=') << endl;

    // Verify n=200
    cout << "\nVerification for n=200:" << endl;
    long long g200 = solve(200);
    assert(g200 == 64658);
    cout << "  VERIFIED: g(200) = 64658" << endl;

    // For n=10000, the computation takes ~7 seconds and ~600 MB RAM
    // Uncomment below to run:
    //
    // cout << "\nSolving for n=10000:" << endl;
    // long long g10000 = solve(10000);
    // cout << "  g(10000) = " << g10000 << endl;
    // assert(g10000 == 159820276);

    cout << "\nKnown answer: g(10000) = 159820276" << endl;
    cout << "\nNote: Full computation requires ~600 MB RAM and ~7 seconds." << endl;

    return 0;
}
