/*
 * Problem 887: Chained Radicals
 * R(a) = sqrt(a + sqrt(a + ...)) = (1 + sqrt(1 + 4a)) / 2.
 */
#include <bits/stdc++.h>
using namespace std;

double nested_radical(double a) {
    return (1.0 + sqrt(1.0 + 4.0 * a)) / 2.0;
}

double nested_iterative(double a, int iters = 100) {
    double x = 0;
    for (int i = 0; i < iters; i++) x = sqrt(a + x);
    return x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << fixed << setprecision(10);
    cout << "=== Nested Radical ===" << endl;
    for (double a : {0.0, 1.0, 2.0, 3.0, 6.0, 12.0, 20.0, 100.0}) {
        double exact = nested_radical(a);
        double iter = nested_iterative(a, 200);
        cout << "a=" << a << ": exact=" << exact << " iter=" << iter
             << " diff=" << scientific << abs(exact - iter) << fixed << endl;
    }

    cout << "\n=== Golden Ratio ===" << endl;
    cout << "R(1) = " << nested_radical(1.0) << endl;
    cout << "phi  = " << (1.0 + sqrt(5.0)) / 2.0 << endl;

    cout << "\n=== Integer Solutions ===" << endl;
    for (int n = 1; n <= 10; n++) {
        int a = n * (n - 1);
        cout << "R(" << a << ") = " << nested_radical(a) << " (n=" << n << ")" << endl;
    }

    cout << "\nAnswer: R(1) = phi = " << nested_radical(1.0) << endl;
    return 0;
}
