#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 667: Moving Pentagon
 *
 * Largest pentagonal table that fits through a unit-wide L-corridor.
 * Requires constrained nonlinear optimization.
 */

struct Point { double x, y; };

double polygon_area(const vector<Point>& v) {
    double area = 0;
    int n = v.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += v[i].x * v[j].y - v[j].x * v[i].y;
    }
    return fabs(area) / 2.0;
}

double width_in_dir(const vector<Point>& v, double theta) {
    double c = cos(theta), s = sin(theta);
    double mn = 1e18, mx = -1e18;
    for (auto& p : v) {
        double proj = p.x * c + p.y * s;
        mn = min(mn, proj);
        mx = max(mx, proj);
    }
    return mx - mn;
}

bool can_fit(const vector<Point>& v, int n_angles = 100) {
    for (int i = 0; i <= n_angles; i++) {
        double theta = i * M_PI / (2 * n_angles);
        if (width_in_dir(v, theta) > 1.0 + 1e-9) return false;
        if (width_in_dir(v, theta + M_PI/2) > 1.0 + 1e-9) return false;
    }
    return true;
}

int main() {
    // Regular pentagon test
    vector<Point> pent(5);
    double scale = 0.45;
    for (int k = 0; k < 5; k++) {
        double a = 2.0 * M_PI * k / 5;
        pent[k] = {scale * cos(a), scale * sin(a)};
    }

    printf("Regular pentagon (scale=%.2f): area=%.6f, fits=%d\n",
           scale, polygon_area(pent), can_fit(pent));

    // Sofa-like pentagon
    vector<Point> sofa = {{-0.5, 0}, {-0.5, 0.5}, {0, 0.6}, {0.5, 0.5}, {0.5, 0}};
    printf("Sofa pentagon: area=%.6f, fits=%d\n",
           polygon_area(sofa), can_fit(sofa));

    printf("Full optimization requires NLP solver.\n");
    return 0;
}
