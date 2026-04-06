/*
 * Problem 890: Birthday Paradox Variants
 * P(n,N) = 1 - prod_{k=0}^{n-1} (1 - k/N).
 */
#include <bits/stdc++.h>
using namespace std;

double birthday_exact(int n, int N) {
    if (n > N) return 1.0;
    double p = 1.0;
    for (int k = 0; k < n; k++) p *= (double)(N - k) / N;
    return 1.0 - p;
}

double birthday_poisson(int n, int N) {
    double lam = (double)n * (n - 1) / (2.0 * N);
    return 1.0 - exp(-lam);
}

int threshold_50(int N) {
    for (int n = 1; n <= N + 1; n++)
        if (birthday_exact(n, N) >= 0.5) return n;
    return N + 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << fixed << setprecision(5);
    cout << "=== Birthday Problem (N=365) ===" << endl;
    for (int n : {5, 10, 20, 23, 30, 50, 70})
        cout << "P(" << n << ",365) = " << birthday_exact(n, 365) << endl;

    cout << "\n=== Thresholds ===" << endl;
    for (int N : {100, 365, 1000, 10000})
        cout << "n_50(" << N << ") = " << threshold_50(N) << endl;

    cout << "\nAnswer: P(23,365) = " << birthday_exact(23, 365) << endl;
    return 0;
}
