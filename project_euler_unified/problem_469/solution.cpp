/*
 * Project Euler Problem 469: Empty Chairs
 *
 * N chairs in a circle. Knights sit one at a time in random available chairs
 * (both neighbors must be empty). E(N) = expected fraction of empty chairs.
 * Find E(10^18) to 14 decimal places.
 *
 * Result: E(N) -> (1 + e^{-2}) / 2 as N -> infinity
 * Answer: 0.56766764161831
 *
 * Compile: g++ -O2 -o solution solution.cpp
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

/*
 * Exact computation for small N using probability tree exploration.
 * State: bitmask of occupied chairs.
 */
#include <map>

double exact_E(int N) {
    // BFS with probabilities
    map<int, double> prob;
    prob[0] = 1.0;

    bool changed = true;
    while (changed) {
        changed = false;
        map<int, double> new_prob;

        for (auto& [state, p] : prob) {
            // Find available chairs
            vector<int> available;
            for (int c = 0; c < N; c++) {
                if (state & (1 << c)) continue;
                int left = (c - 1 + N) % N;
                int right = (c + 1) % N;
                if ((state & (1 << left)) || (state & (1 << right))) continue;
                available.push_back(c);
            }

            if (available.empty()) {
                new_prob[state] += p;
            } else {
                changed = true;
                double each = p / available.size();
                for (int c : available) {
                    new_prob[state | (1 << c)] += each;
                }
            }
        }
        prob = new_prob;
    }

    double expected = 0.0;
    for (auto& [state, p] : prob) {
        int occ = __builtin_popcount(state);
        double empty_frac = (double)(N - occ) / N;
        expected += p * empty_frac;
    }
    return expected;
}

/*
 * Monte Carlo simulation for moderate N.
 */
double monte_carlo_E(int N, int trials = 100000) {
    mt19937 rng(42);
    double total = 0.0;

    for (int t = 0; t < trials; t++) {
        vector<bool> occupied(N, false);

        while (true) {
            vector<int> available;
            for (int c = 0; c < N; c++) {
                if (occupied[c]) continue;
                int left = (c - 1 + N) % N;
                int right = (c + 1) % N;
                if (occupied[left] || occupied[right]) continue;
                available.push_back(c);
            }
            if (available.empty()) break;
            int idx = uniform_int_distribution<int>(0, available.size() - 1)(rng);
            occupied[available[idx]] = true;
        }

        int empty_count = count(occupied.begin(), occupied.end(), false);
        total += (double)empty_count / N;
    }
    return total / trials;
}

/*
 * High-precision computation of (1 + e^{-2}) / 2 using Taylor series.
 */
long double compute_answer() {
    // e^{-2} via Taylor series: sum_{k=0}^{inf} (-2)^k / k!
    long double e_neg2 = 0.0L;
    long double term = 1.0L;
    for (int k = 0; k < 100; k++) {
        e_neg2 += term;
        term *= -2.0L / (k + 1);
    }
    return (1.0L + e_neg2) / 2.0L;
}

/*
 * Derivation of the limiting value:
 *
 * This is a random sequential adsorption (RSA) problem on a cycle.
 * Each knight occupies one chair and blocks both neighbors.
 *
 * For the line version of length n, let L(n) = expected occupancy.
 * The recurrence is:
 *   L(n) = 1 + (2/n) * sum_{j=0}^{n-3} L(j) + (1/n)*L(n-2)
 *
 * For the continuous analog (Renyi's parking problem with interval size 2
 * on a line of length n), the ODE gives:
 *   M'(x) = 1 + (2/x) * integral_0^{x-2} M(t) dt
 *
 * The solution yields: as n -> inf,
 *   L(n)/n -> (1 - e^{-2}) / 2
 *
 * Since fraction of occupied = L(N)/N -> (1-e^{-2})/2,
 * fraction of empty = 1 - (1-e^{-2})/2 = (1+e^{-2})/2.
 *
 * For the cycle vs. line: the correction is O(1/N), negligible at N=10^18.
 */

int main() {
    cout << fixed << setprecision(14);
    cout << "Project Euler 469: Empty Chairs" << endl;
    cout << "================================" << endl << endl;

    // Verify small cases
    cout << "Exact values:" << endl;
    for (int N = 4; N <= 14; N += 2) {
        double e = exact_E(N);
        cout << "  E(" << N << ") = " << e << endl;
    }
    cout << endl;

    cout << "Expected: E(4) = " << 1.0/2.0 << ", E(6) = " << 5.0/9.0 << endl;
    cout << endl;

    // Monte Carlo for larger N
    cout << "Monte Carlo:" << endl;
    for (int N : {20, 50, 100, 200, 500, 1000}) {
        double e = monte_carlo_E(N, 50000);
        cout << "  E(" << N << ") ~ " << e << endl;
    }
    cout << endl;

    // Compute answer
    long double answer = compute_answer();
    cout << "Limiting value (1 + e^{-2}) / 2 = " << (double)answer << endl;
    cout << endl;

    // Detailed computation
    long double e_neg2 = expl(-2.0L);
    long double exact = (1.0L + e_neg2) / 2.0L;
    cout << "e^{-2} = " << (double)e_neg2 << endl;
    cout << "(1 + e^{-2}) / 2 = " << (double)exact << endl;
    cout << endl;

    cout << "========================================" << endl;
    cout << "Answer: E(10^18) = " << (double)exact << endl;
    cout << "Answer: 0.56766764161831" << endl;
    cout << "========================================" << endl;

    return 0;
}
