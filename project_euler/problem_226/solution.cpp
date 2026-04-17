#include <bits/stdc++.h>
using namespace std;

// Triangle wave: distance to nearest integer
double s(double x) {
    return abs(x - round(x));
}

// Blancmange curve value at x (60 terms suffice for ~18 digit precision)
double blanc(double x) {
    double result = 0.0;
    double pow2 = 1.0;
    for (int n = 0; n < 60; n++) {
        result += s(pow2 * x) / pow2;
        pow2 *= 2.0;
    }
    return result;
}

// Circle: (x - 1/4)^2 + (y - 1/2)^2 = 1/16
// Lower arc: y = 1/2 - sqrt(1/16 - (x - 1/4)^2)
double circle_lower(double x) {
    double val = 1.0 / 16.0 - (x - 0.25) * (x - 0.25);
    if (val < 0) return 0.5;
    return 0.5 - sqrt(val);
}

// Function whose root gives the intersection point
double on_circle(double x) {
    double b = blanc(x);
    return (x - 0.25) * (x - 0.25) + (b - 0.5) * (b - 0.5) - 1.0 / 16.0;
}

// Brent's method to find root in [a, b]
double brent(double a, double b, double tol = 1e-15) {
    double fa = on_circle(a), fb = on_circle(b);
    if (fa * fb > 0) return a;
    double c = a, fc = fa, d = b - a, e = d;
    for (int i = 0; i < 200; i++) {
        if (fb * fc > 0) { c = a; fc = fa; d = e = b - a; }
        if (fabs(fc) < fabs(fb)) { a = b; b = c; c = a; fa = fb; fb = fc; fc = fa; }
        double tol1 = 2e-16 * fabs(b) + 0.5 * tol;
        double m = 0.5 * (c - b);
        if (fabs(m) <= tol1 || fb == 0) return b;
        if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
            double s_val;
            if (a == c) {
                s_val = fb / fa; double p = 2 * m * s_val; double q = 1 - s_val;
                if (p > 0) q = -q; else p = -p;
                if (2*p < 3*m*q - fabs(tol1*q) && 2*p < fabs(e*q)) { e = d; d = p/q; }
                else { d = m; e = m; }
            } else { d = m; e = m; }
        } else { d = m; e = m; }
        a = b; fa = fb;
        if (fabs(d) > tol1) b += d;
        else b += (m > 0 ? tol1 : -tol1);
        fb = on_circle(b);
    }
    return b;
}

int main() {
    // Find intersection point x1
    double x1 = brent(0.05, 0.1);
    double x2 = 0.5;

    // Integrand: blanc(x) - circle_lower(x)
    auto integrand = [](double x) -> double {
        return blanc(x) - (0.5 - sqrt(max(0.0, 1.0 / 16.0 - (x - 0.25) * (x - 0.25))));
    };

    // Simpson's rule with 2M subdivisions
    int N = 2000000;
    double h = (x2 - x1) / N;
    double sum = integrand(x1) + integrand(x2);

    for (int i = 1; i < N; i++) {
        double x = x1 + i * h;
        sum += (i % 2 == 0 ? 2.0 : 4.0) * integrand(x);
    }

    double area = sum * h / 3.0;
    printf("%.8f\n", area);
    return 0;
}
