#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 666: Polymorphic Bacteria
 *
 * Multi-type Galton-Watson branching process.
 * Extinction probability via fixed-point iteration of PGFs.
 *
 * r_0 = 306, r_{n+1} = r_n^2 mod 10007
 * Rule: q = r[i*m+j] % 5 determines action.
 */

int main() {
    int k = 500, m = 10;

    // Generate random sequence
    vector<int> r(k * m + 10);
    r[0] = 306;
    for (int i = 1; i < (int)r.size(); i++)
        r[i] = (1LL * r[i-1] * r[i-1]) % 10007;

    // Rules for each type
    vector<vector<int>> rules(k, vector<int>(m));
    for (int i = 0; i < k; i++)
        for (int j = 0; j < m; j++)
            rules[i][j] = r[i * m + j] % 5;

    // Fixed-point iteration
    vector<double> q(k, 0.0);
    for (int iter = 0; iter < 2000; iter++) {
        vector<double> q_new(k);
        double max_diff = 0;
        for (int i = 0; i < k; i++) {
            double val = 0;
            for (int j = 0; j < m; j++) {
                int rule = rules[i][j];
                if (rule == 0) val += 1.0;
                else if (rule == 1) val += q[i];
                else if (rule == 2) val += q[(i+1) % k];
                else if (rule == 3) val += q[i] * q[i] * q[i];
                else if (rule == 4) val += q[i] * q[(i+1) % k];
            }
            q_new[i] = val / m;
            max_diff = max(max_diff, fabs(q_new[i] - q[i]));
        }
        q = q_new;
        if (max_diff < 1e-12) {
            printf("Converged at iteration %d\n", iter);
            break;
        }
    }

    printf("P(%d,%d) = %.8f\n", k, m, q[0]);
    return 0;
}
