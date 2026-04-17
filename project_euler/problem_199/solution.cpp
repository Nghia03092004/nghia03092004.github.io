#include <bits/stdc++.h>
using namespace std;

int main() {
    // Three unit circles (curvature 1) inside an outer circle
    // Outer circle radius R = 1 + 2/sqrt(3)
    double R = 1.0 + 2.0 / sqrt(3.0);
    double k_outer = -1.0 / R;

    // Initial gaps:
    // 3 outer gaps: (k_outer, 1, 1)
    // 1 inner gap: (1, 1, 1)

    // Each gap is a triple of curvatures
    // We use Descartes' theorem: k4 = k1+k2+k3 + 2*sqrt(k1*k2+k2*k3+k3*k1)

    double total_area = 3.0 * M_PI; // three unit circles

    // Use a list of gap triples
    vector<tuple<double, double, double>> gaps;
    // 3 outer gaps
    for (int i = 0; i < 3; i++)
        gaps.push_back({k_outer, 1.0, 1.0});
    // 1 inner gap
    gaps.push_back({1.0, 1.0, 1.0});

    for (int iter = 0; iter < 10; iter++) {
        vector<tuple<double, double, double>> new_gaps;
        for (auto& [a, b, c] : gaps) {
            double k_new = a + b + c + 2.0 * sqrt(a * b + b * c + c * a);
            total_area += M_PI / (k_new * k_new);
            new_gaps.push_back({a, b, k_new});
            new_gaps.push_back({a, c, k_new});
            new_gaps.push_back({b, c, k_new});
        }
        gaps = move(new_gaps);
    }

    double outer_area = M_PI * R * R;
    double fraction = 1.0 - total_area / outer_area;

    cout << fixed << setprecision(8) << fraction << endl;
    return 0;
}
