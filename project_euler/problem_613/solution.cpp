#include <bits/stdc++.h>
using namespace std;
int main() {
    // Monte Carlo for Pythagorean ant in 3-4-5 triangle
    double a = 3, b = 4, c = 5;
    int N = 1000000;
    mt19937 rng(42);
    uniform_real_distribution<double> dist(0, 1);
    int count = 0;
    for (int i = 0; i < N; i++) {
        double u = dist(rng), v = dist(rng);
        if (u + v > 1) { u = 1-u; v = 1-v; }
        double x = a*u, y = b*v;
        double dh = abs(b*x + a*y - a*b) / c;
        if (dh < min(y, x)) count++;
    }
    cout << fixed << setprecision(4) << (double)count / N << endl;
    return 0;
}
