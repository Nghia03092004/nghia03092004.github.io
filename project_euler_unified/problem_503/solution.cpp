#include <bits/stdc++.h>
using namespace std;

int main() {
    // Compute sum of P(n) = H_n / n for n = 1..N
    // where H_n is the n-th harmonic number
    const int N = 1000;
    double harmonic = 0.0;
    double total = 0.0;

    for (int n = 1; n <= N; n++) {
        harmonic += 1.0 / n;
        total += harmonic / n;  // P(n) = H_n / n
    }

    cout << fixed << setprecision(10) << total << endl;

    // Verify with backward induction DP for small n
    int test_n = 5;
    int R = 200;
    vector<double> v(R + 1, 0.0);
    for (int r = 1; r <= R; r++) {
        double best = 0.0;
        for (int s = 1; s <= test_n; s++) {
            double p = (double)s / test_n;
            double payoff = p * ((double)s / test_n) + (1 - p) * v[r - 1];
            best = max(best, payoff);
        }
        v[r] = best;
    }
    cout << "DP verification for n=" << test_n << ": " << v[R] << endl;

    return 0;
}
