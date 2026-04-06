#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 144: Laser reflections in ellipse 4x^2 + y^2 = 100
    // Entry: (0, 10.1) -> first hit: (1.4, -9.6)
    // Exit: |x| <= 0.01 at top (y > 0)

    double x0 = 0.0, y0 = 10.1;
    double x1 = 1.4, y1 = -9.6;

    int count = 0;

    while (true) {
        // Direction of incoming beam
        double dx = x1 - x0;
        double dy = y1 - y0;

        // Normal at (x1, y1): proportional to (4*x1, y1)
        double nx = 4.0 * x1;
        double ny = y1;

        // Reflect: d' = d - 2*(d.n)/(n.n) * n
        double dn = dx * nx + dy * ny;
        double nn = nx * nx + ny * ny;
        double rx = dx - 2.0 * dn / nn * nx;
        double ry = dy - 2.0 * dn / nn * ny;

        // Find next intersection with 4x^2 + y^2 = 100
        // Ray: (x1 + t*rx, y1 + t*ry)
        // 4(x1+t*rx)^2 + (y1+t*ry)^2 = 100
        // t * (4*rx^2 + ry^2) + 2*(4*x1*rx + y1*ry) = 0  (dividing out t=0)
        double denom = 4.0 * rx * rx + ry * ry;
        double t = -2.0 * (4.0 * x1 * rx + y1 * ry) / denom;

        double x2 = x1 + t * rx;
        double y2 = y1 + t * ry;

        count++;

        // Check exit condition
        if (fabs(x2) <= 0.01 && y2 > 0) {
            break;
        }

        x0 = x1; y0 = y1;
        x1 = x2; y1 = y2;
    }

    cout << count << endl;
    return 0;
}
