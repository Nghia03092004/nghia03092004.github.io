#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 723: Pythagorean Quadrilaterals
 *
 * Count pythagorean lattice grid quadrilaterals inscribed in circles.
 * A quadrilateral ABCD with vertices on x^2+y^2=d is pythagorean if
 * a^2 + b^2 + c^2 + d^2 = 8d (sides squared sum to 8r^2).
 *
 * S(n) = sum_{d|n} f(sqrt(d))
 *
 * The approach uses:
 * 1. Enumerate lattice points on circle via sum-of-two-squares
 * 2. Count valid quadrilaterals satisfying the pythagorean condition
 * 3. Multiplicative structure for the large input
 */

vector<pair<int,int>> lattice_points(int d) {
    vector<pair<int,int>> pts;
    int s = (int)sqrt((double)d) + 1;
    for (int x = -s; x <= s; x++) {
        int y2 = d - x*x;
        if (y2 < 0) continue;
        int y = (int)round(sqrt((double)y2));
        if (y*y == y2) {
            pts.push_back({x, y});
            if (y > 0) pts.push_back({x, -y});
        }
    }
    return pts;
}

int main() {
    // Verify small cases
    for (int d : {1, 2, 5}) {
        auto pts = lattice_points(d);
        int n = pts.size();
        int count = 0;
        for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) if (j != i)
        for (int k = 0; k < n; k++) if (k != i && k != j)
        for (int l = 0; l < n; l++) if (l != i && l != j && l != k) {
            auto [x1,y1] = pts[i]; auto [x2,y2] = pts[j];
            auto [x3,y3] = pts[k]; auto [x4,y4] = pts[l];
            long long a2 = (long long)(x1-x2)*(x1-x2) + (long long)(y1-y2)*(y1-y2);
            long long b2 = (long long)(x2-x3)*(x2-x3) + (long long)(y2-y3)*(y2-y3);
            long long c2 = (long long)(x3-x4)*(x3-x4) + (long long)(y3-y4)*(y3-y4);
            long long d2 = (long long)(x4-x1)*(x4-x1) + (long long)(y4-y1)*(y4-y1);
            if (a2 + b2 + c2 + d2 == 8LL * d) count++;
        }
        printf("f(sqrt(%d)) = %d\n", d, count / 8);
    }
    return 0;
}
