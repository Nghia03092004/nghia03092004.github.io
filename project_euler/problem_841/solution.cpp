#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 841: Regular Star Polygons
 *
 * Sum of areas of all valid star polygons {n/k} inscribed in unit circle.
 * Area({n/k}) = (n/2) * sin(2*pi*k/n)
 * Valid when gcd(n,k) = 1 and 1 < k < n/2.
 */

int main() {
    int N = 10000;
    double total = 0.0;
    const double PI = acos(-1.0);

    for (int n = 3; n <= N; n++) {
        for (int k = 2; k < n / 2 + 1; k++) {
            if (n % 2 == 0 && k == n / 2) continue;
            if (__gcd(n, k) == 1) {
                total += (n / 2.0) * sin(2.0 * PI * k / n);
            }
        }
    }

    // Verify small case: {5/2} pentagram
    double pent = 2.5 * sin(2.0 * PI * 2 / 5);
    assert(abs(pent - 1.46946) < 0.001);

    printf("%.3f\n", total);
    return 0;
}
