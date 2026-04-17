#include <bits/stdc++.h>
using namespace std;

typedef long double ld;

// Area of {x >= c, y >= c, x^2+y^2 <= r^2}
// = integral_c^sqrt(r^2-c^2) (sqrt(r^2-x^2) - c) dx
// = [x*sqrt(r^2-x^2)/2 + r^2/2*arcsin(x/r)]_c^sqrt(r^2-c^2) - c*(sqrt(r^2-c^2)-c)
ld circle_quad_area(ld r, ld c) {
    if (r * r < 2 * c * c) return 0.0L;

    ld x_max = sqrtl(r * r - c * c);

    auto F = [&](ld x) -> ld {
        return x * sqrtl(r * r - x * x) / 2 + r * r / 2 * asinl(x / r);
    };

    ld area = F(x_max) - F(c) - c * (x_max - c);
    return max(area, (ld)0);
}

int main() {
    ld total = 0;
    for (int k = 1; k <= 100000; k++) {
        ld c = 1.0L / k;
        ld r1 = (k - 0.5L) / k;
        ld r2 = (k + 0.5L) / k;

        ld a2 = circle_quad_area(r2, c);
        ld a1 = circle_quad_area(r1, c);
        ld area = a2 - a1;

        total += k * area;
    }
    cout << fixed << setprecision(5) << (double)total << endl;
    return 0;
}
