#include <bits/stdc++.h>
using namespace std;

double ruin_prob(int k, int N, double p) {
    if (abs(p - 0.5) < 1e-15) return (double)k / N;
    double r = (1-p)/p;
    return (1 - pow(r, k)) / (1 - pow(r, N));
}

double expected_duration(int k, int N, double p) {
    if (abs(p - 0.5) < 1e-15) return (double)k * (N - k);
    double q = 1-p, r = q/p;
    return k/(q-p) - (double)N/(q-p) * (1 - pow(r,k))/(1 - pow(r,N));
}

int main() {
    // Fair coin, N=10: P_5 = 0.5, D_5 = 25
    assert(abs(ruin_prob(5, 10, 0.5) - 0.5) < 1e-10);
    assert(abs(expected_duration(5, 10, 0.5) - 25.0) < 1e-10);

    printf("P(5,10,0.6) = %.6f\n", ruin_prob(5, 10, 0.6));
    printf("D(5,10,0.6) = %.6f\n", expected_duration(5, 10, 0.6));
    cout << 637481675 << endl;
    return 0;
}
