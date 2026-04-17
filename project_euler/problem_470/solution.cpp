/*
 * Project Euler Problem 470: Super Ramvok
 *
 * Compute F(20) = sum_{d=4}^{20} sum_{c=0}^{20} S(d, c)
 * where S(d,c) is the expected profit of optimally-played Super Ramvok.
 *
 * Approach:
 *   For each (d, c):
 *   1. Compute R(S, c) for all subsets S of {1,...,d} (optimal Ramvok profit)
 *   2. Solve linear system on 2^d states for V(S) (Super Ramvok value)
 *   3. S(d, c) = V({1,...,d})
 *
 * Compile: g++ -O2 -o solution solution.cpp -lm
 * For d>=16, needs ~2^d memory and sparse solver.
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

/*
 * Compute R(visible_values, c) = optimal expected profit of Ramvok.
 * visible_values: sorted list of face values that are not blank.
 * c: cost per turn.
 */
double compute_R(const vector<int>& vals, double c) {
    if (vals.empty()) return 0.0;

    int k = vals.size();
    double mu = 0;
    for (int v : vals) mu += v;
    mu /= k;

    double E_prev = mu;
    double best = max(0.0, E_prev - c);

    for (int t = 2; t < 1000; t++) {
        double E_t = 0;
        for (int v : vals) E_t += max((double)v, E_prev);
        E_t /= k;

        double profit = E_t - c * t;
        best = max(best, profit);

        if (E_t - E_prev < c) break;
        E_prev = E_t;
    }

    return max(best, 0.0);
}

/*
 * Get visible values for a given mask and die size d.
 * Face i (0-indexed) has value i+1.
 */
vector<int> get_visible(int mask, int d) {
    vector<int> vals;
    for (int i = 0; i < d; i++) {
        if (mask & (1 << i)) vals.push_back(i + 1);
    }
    return vals;
}

/*
 * Solve Super Ramvok for die size d and cost c.
 *
 * Linear system: V(S) - (1/d) * sum_i V(S^(1<<i)) = R(S)
 * with V(0) = 0.
 *
 * For small d (<= 20), use Gauss-Seidel with SOR (Successive Over-Relaxation).
 * The system is well-conditioned when V(0)=0 boundary is imposed.
 *
 * Actually, for this specific problem, we can use a clever observation:
 * The hypercube graph has known spectral properties. But let's just iterate.
 *
 * For convergence, we use a modified iteration. Note that the system is:
 * V(S) = R(S) + (1/d) * sum_i V(S^(1<<i))
 *
 * where transitions to state 0 contribute 0. This means the iteration
 * V^{new}(S) = R(S) + (1/d) * sum_i V^{old}(S^(1<<i))
 *
 * converges if the spectral radius of the iteration matrix < 1.
 * On the punctured hypercube (without vertex 0), the largest eigenvalue
 * of the adjacency matrix / d is (d-2)/d (the eigenvalue d/d = 1 requires
 * the all-ones vector which is not in the punctured space).
 * So spectral radius = (d-2)/d < 1 for d >= 3. Convergence guaranteed!
 */
double solve_super_ramvok(int d, double c) {
    int n_states = 1 << d;
    int full = n_states - 1;

    // Precompute R for all states
    vector<double> R(n_states, 0.0);
    for (int S = 1; S < n_states; S++) {
        vector<int> vals = get_visible(S, d);
        R[S] = compute_R(vals, c);
    }

    // Iterative solve
    vector<double> V(n_states, 0.0);
    double inv_d = 1.0 / d;

    for (int iter = 0; iter < 100000; iter++) {
        double max_change = 0.0;

        for (int S = 1; S < n_states; S++) {
            double neighbor_sum = 0.0;
            for (int i = 0; i < d; i++) {
                neighbor_sum += V[S ^ (1 << i)];
            }
            double new_V = R[S] + neighbor_sum * inv_d;
            double change = fabs(new_V - V[S]);
            max_change = max(max_change, change);
            V[S] = new_V;
        }

        if (max_change < 1e-12) {
            // cout << "  Converged in " << iter + 1 << " iterations" << endl;
            break;
        }
    }

    return V[full];
}

int main() {
    cout << fixed << setprecision(4);
    cout << "Project Euler 470: Super Ramvok" << endl;
    cout << "================================" << endl << endl;

    // Verify examples
    cout << "Verification:" << endl;
    double r4 = compute_R({1, 2, 3, 4}, 0.2);
    cout << "  R(4, 0.2) = " << r4 << " (expected 2.65)" << endl;

    double s6 = solve_super_ramvok(6, 1.0);
    cout << "  S(6, 1) = " << setprecision(1) << s6 << " (expected 208.3)" << endl;
    cout << endl;

    // Compute F(n) for small n first
    cout << setprecision(4);
    int n_max = 10; // Start small, increase to 20 for full solution

    cout << "Computing F(" << n_max << ")..." << endl;
    double F = 0.0;
    for (int d = 4; d <= n_max; d++) {
        double d_total = 0.0;
        for (int c_int = 0; c_int <= n_max; c_int++) {
            double c = (double)c_int;
            double s = solve_super_ramvok(d, c);
            d_total += s;
            if (c_int <= 3) {
                cout << "  S(" << d << ", " << c_int << ") = " << s << endl;
            }
        }
        F += d_total;
        cout << "  d=" << d << " subtotal: " << d_total
             << ", running F = " << F << endl;
    }
    cout << endl;
    cout << "F(" << n_max << ") = " << F << endl;
    cout << "F(" << n_max << ") rounded = " << (long long)round(F) << endl;
    cout << endl;

    // For F(20), uncomment below (requires more time and memory for d=17..20)
    /*
    cout << "Computing F(20)..." << endl;
    F = 0.0;
    for (int d = 4; d <= 20; d++) {
        double d_total = 0.0;
        for (int c_int = 0; c_int <= 20; c_int++) {
            double c = (double)c_int;
            double s = solve_super_ramvok(d, c);
            d_total += s;
        }
        F += d_total;
        cout << "  d=" << d << " subtotal: " << d_total
             << ", running F = " << F << endl;
    }
    cout << "F(20) = " << F << endl;
    cout << "F(20) rounded = " << (long long)round(F) << endl;
    */

    return 0;
}
