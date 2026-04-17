#include <bits/stdc++.h>
using namespace std;

// Rolling ellipse: compute center path and arc length

// Numerical integration using Simpson's rule
double simpson(function<double(double)> f, double a, double b, int n) {
    double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; i += 2) s += 4 * f(a + i * h);
    for (int i = 2; i < n; i += 2) s += 2 * f(a + i * h);
    return s * h / 3.0;
}

// Arc length of ellipse from 0 to theta
double arc_length(double a, double b, double theta) {
    auto integrand = [a, b](double t) {
        return sqrt(a * a * sin(t) * sin(t) + b * b * cos(t) * cos(t));
    };
    return simpson(integrand, 0, theta, 10000);
}

// Circumference of ellipse
double circumference(double a, double b) {
    return arc_length(a, b, 2 * M_PI);
}

int main() {
    double a = 2.0, b = 1.0;

    cout << fixed << setprecision(8);
    cout << "Ellipse a=" << a << ", b=" << b << endl;
    cout << "Circumference = " << circumference(a, b) << endl;

    // Compute center path
    int N = 10000;
    vector<double> X(N), Y(N);

    for (int i = 0; i < N; i++) {
        double theta = 2.0 * M_PI * i / (N - 1);

        // Arc length up to theta
        double s = arc_length(a, b, theta);

        // Height of center
        double denom = sqrt(a * a * sin(theta) * sin(theta) + b * b * cos(theta) * cos(theta));
        double h = a * b / denom;

        // Tangent angle
        double tx = -a * sin(theta);
        double ty = b * cos(theta);
        double alpha = atan2(ty, tx);

        // Center offset from contact
        double dx_ell = -a * cos(theta);
        double dy_ell = -b * sin(theta);

        double rot = -alpha + M_PI / 2.0;
        double cr = cos(rot), sr = sin(rot);

        X[i] = s + dx_ell * cr - dy_ell * sr;
        Y[i] = abs(dx_ell * sr + dy_ell * cr);
    }

    // Compute arc length of center path
    double total_arc = 0;
    for (int i = 1; i < N; i++) {
        double dx = X[i] - X[i-1];
        double dy = Y[i] - Y[i-1];
        total_arc += sqrt(dx * dx + dy * dy);
    }

    cout << "Center path arc length = " << total_arc << endl;

    return 0;
}
