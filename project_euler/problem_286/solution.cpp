#include <bits/stdc++.h>
using namespace std;

// Compute P(exactly 20 scores in 50 shots) given q
double compute(double q) {
    // dp[j] = probability of exactly j scores so far
    vector<double> dp(21, 0.0);
    dp[0] = 1.0;
    for (int i = 1; i <= 50; i++) {
        double p_score = 1.0 - (double)i / q;  // probability of scoring shot i
        double p_miss = (double)i / q;
        // Update in reverse to avoid overwriting
        for (int j = min(i, 20); j >= 1; j--) {
            dp[j] = dp[j] * p_miss + dp[j - 1] * p_score;
        }
        dp[0] *= p_miss;
    }
    return dp[20];
}

int main() {
    double lo = 50.0, hi = 100.0;
    double target = 0.02;

    // Binary search for q
    for (int iter = 0; iter < 200; iter++) {
        double mid = (lo + hi) / 2.0;
        double val = compute(mid);
        if (val > target) {
            lo = mid;  // need larger q to decrease P(X=20)
        } else {
            hi = mid;
        }
    }

    printf("%.10f\n", (lo + hi) / 2.0);
    return 0;
}
