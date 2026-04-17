#include <bits/stdc++.h>
using namespace std;

int main() {
    // L-section area (one quarter)
    double L_area = 1.0 - M_PI / 4.0;
    double threshold = 0.001; // 0.1%

    for (int n = 1; ; n++) {
        double s = 1.0 / n; // slope

        // Find intersection of line y = x/n with circle (x-1)^2 + (y-1)^2 = 1
        // (1 + s^2)x^2 - 2(1+s)x + 1 = 0
        double a = 1.0 + s * s;
        double b = -2.0 * (1.0 + s);
        double c = 1.0;
        double disc = b * b - 4.0 * a * c;
        double x0 = (-b - sqrt(disc)) / (2.0 * a);
        double y0 = x0 * s;

        // Area of concave triangle:
        // Part 1: triangle under the line from 0 to x0
        double area1 = x0 * y0 / 2.0;

        // Part 2: area between circle bottom and x-axis from x0 to 1
        // Circle bottom: y = 1 - sqrt(1 - (x-1)^2)
        // Integral of (1 - sqrt(1 - (x-1)^2)) dx from x0 to 1
        // Let u = x - 1, du = dx
        // Integral of (1 - sqrt(1 - u^2)) du from (x0-1) to 0
        // = [u - u*sqrt(1-u^2)/2 - arcsin(u)/2] from (x0-1) to 0

        double u0 = x0 - 1.0;
        double val_at_u0 = u0 - u0 * sqrt(1.0 - u0 * u0) / 2.0 - asin(u0) / 2.0;
        double val_at_0 = 0.0;
        double area2 = val_at_0 - val_at_u0;

        double concave_area = area1 + area2;
        double ratio = concave_area / L_area;

        if (ratio < threshold) {
            cout << n << endl;
            break;
        }
    }

    return 0;
}
