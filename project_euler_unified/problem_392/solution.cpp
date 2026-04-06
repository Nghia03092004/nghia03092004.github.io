#include <bits/stdc++.h>
using namespace std;

int main() {
    // N = 400 inner gridlines; by symmetry use N/2 = 200 in [0,1]
    const int N = 400;
    const int half = N / 2;
    const int n_strips = half + 1;  // 201 strips in [0,1]

    double lo = 1e-15, up = 1.0 - 1e-15;

    // Binary search on x_1 so that the recurrence lands at x_{n_strips} = 1
    for (int iter = 0; iter < 500; iter++) {
        double mid = (lo + up) / 2.0;
        double x_prev = 0.0;
        double x_curr = mid;
        double area = x_curr;  // (x_1 - 0) * sqrt(1 - 0^2) = x_1
        bool ok = true;

        for (int i = 2; i <= n_strips; i++) {
            double s1 = sqrt(1.0 - x_curr * x_curr);
            double s2 = sqrt(1.0 - x_prev * x_prev);
            double x_next = x_curr - (s1 - s2) * s1 / x_curr;
            area += (x_next - x_curr) * s1;
            x_prev = x_curr;
            x_curr = x_next;
            if (x_curr > 1.0) {
                ok = false;
                break;
            }
        }

        if (!ok) up = mid;
        else lo = mid;
    }

    // Final computation with converged x_1
    double x1 = (lo + up) / 2.0;
    double x_prev = 0.0, x_curr = x1;
    double area = x_curr;
    for (int i = 2; i <= n_strips; i++) {
        double s1 = sqrt(1.0 - x_curr * x_curr);
        double s2 = sqrt(1.0 - x_prev * x_prev);
        double x_next = x_curr - (s1 - s2) * s1 / x_curr;
        area += (x_next - x_curr) * s1;
        x_prev = x_curr;
        x_curr = x_next;
    }

    cout << fixed << setprecision(10) << 4.0 * area << endl;
    return 0;
}
