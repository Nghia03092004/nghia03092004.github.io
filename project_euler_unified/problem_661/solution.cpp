#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 661: A Long Chess Match
 *
 * Compute H(n) = sum_{k=3}^{n} E_A(1/sqrt(k+3), 1/sqrt(k+3)+1/k^2, 1/k^3)
 *
 * E_A uses the Wiener-Hopf factorization:
 *   r_plus = (1 - sqrt(1 - 4*pA*pB)) / (2*pB)
 *   E_A = r_plus * z / ((1 - z) * (1 - r_plus * z))  where z = 1 - p
 *
 * Two methods: closed-form and truncated Markov chain (cross-check).
 */

// Method 1: Closed-form via generating function
double expected_leading_closed(double pA, double pB, double p) {
    double disc = 1.0 - 4.0 * pA * pB;
    if (disc < 0) disc = 0.0;
    double r_plus = (1.0 - sqrt(disc)) / (2.0 * pB);
    double z = 1.0 - p;
    double denom = (1.0 - z) * (1.0 - r_plus * z);
    if (fabs(denom) < 1e-30) return 0.0;
    return r_plus * z / denom;
}

// Method 2: Direct DP summation (for verification on small cases)
double expected_leading_dp(double pA, double pB, double p, int M = 200) {
    int size = 2 * M + 1;
    double surv = 1.0 - p;
    double a = pB * surv;      // move left
    double b = (1 - pA - pB) * surv; // stay
    double c = pA * surv;      // move right

    vector<double> prob(size, 0.0), new_prob(size, 0.0);
    prob[M] = 1.0; // start at d = 0

    double result = 0.0;
    int T_max = min((int)(10.0 / p), 50000);

    for (int t = 1; t <= T_max; t++) {
        fill(new_prob.begin(), new_prob.end(), 0.0);
        for (int i = 1; i < size - 1; i++) {
            new_prob[i] = prob[i-1] * c + prob[i] * b + prob[i+1] * a;
        }
        swap(prob, new_prob);
        double pos_prob = 0.0;
        for (int i = M + 1; i < size; i++) pos_prob += prob[i];
        result += pos_prob;
        if (pos_prob < 1e-15) break;
    }
    return result;
}

int main() {
    // Compute H(50)
    double H = 0.0;
    for (int k = 3; k <= 50; k++) {
        double pA = 1.0 / sqrt(k + 3.0);
        double pB = pA + 1.0 / ((double)k * k);
        double p = 1.0 / ((double)k * k * k);
        double ea = expected_leading_closed(pA, pB, p);
        H += ea;
    }

    printf("H(50) = %.4f\n", H);

    // Cross-check: known example
    double check = expected_leading_closed(0.25, 0.25, 0.5);
    printf("E_A(0.25,0.25,0.5) = %.6f (expected ~0.585786)\n", check);
    assert(fabs(check - 0.585786) < 0.001);

    // Cross-check H(3)
    double H3 = 0.0;
    for (int k = 3; k <= 3; k++) {
        double pA = 1.0 / sqrt(k + 3.0);
        double pB = pA + 1.0 / ((double)k * k);
        double p = 1.0 / ((double)k * k * k);
        H3 += expected_leading_closed(pA, pB, p);
    }
    printf("H(3) = %.4f (expected ~6.8345)\n", H3);

    return 0;
}
