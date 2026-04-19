#include <bits/stdc++.h>
using namespace std;

namespace {

using ld = long double;

struct Point {
    ld x = 0;
    ld y = 0;

    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }
    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }
    Point operator*(ld scale) const {
        return {x * scale, y * scale};
    }
};

ld dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

ld norm(const Point& a) {
    return sqrtl(dot(a, a));
}

Point left_normal(const Point& v) {
    return {-v.y, v.x};
}

struct Pyramid {
    array<Point, 4> base;
    Point center;
    ld height = 0;
};

Pyramid read_pyramid() {
    long long x1, y1, x2, y2, h;
    cin >> x1 >> y1 >> x2 >> y2 >> h;

    Point a{static_cast<ld>(x1), static_cast<ld>(y1)};
    Point b{static_cast<ld>(x2), static_cast<ld>(y2)};
    Point edge = b - a;
    ld side = norm(edge);
    Point inside = left_normal(edge) * (1.0L / side);

    Pyramid pyramid;
    pyramid.base[0] = a;
    pyramid.base[1] = b;
    pyramid.base[2] = b + inside * side;
    pyramid.base[3] = a + inside * side;
    pyramid.center = (pyramid.base[0] + pyramid.base[2]) * 0.5L;
    pyramid.height = h;
    return pyramid;
}

void solve() {
    Pyramid first = read_pyramid();
    Pyramid second = read_pyramid();

    auto point_on_edge = [](const Point& a, const Point& b, ld t) {
        return a + (b - a) * t;
    };

    auto distance_to_apex = [](const Point& center, ld height, const Point& p) {
        Point diff = p - center;
        return sqrtl(dot(diff, diff) + height * height);
    };

    ld answer = 1e100L;
    for (int i = 0; i < 4; ++i) {
        Point e1 = first.base[i];
        Point e2 = first.base[(i + 1) % 4];
        for (int j = 0; j < 4; ++j) {
            Point f1 = second.base[j];
            Point f2 = second.base[(j + 1) % 4];

            auto value = [&](ld x, ld y) {
                Point p = point_on_edge(e1, e2, x);
                Point q = point_on_edge(f1, f2, y);
                return distance_to_apex(first.center, first.height, p) +
                       norm(p - q) +
                       distance_to_apex(second.center, second.height, q);
            };

            auto best_for_x = [&](ld x) {
                ld low = 0;
                ld high = 1;
                for (int it = 0; it < 80; ++it) {
                    ld m1 = (2 * low + high) / 3;
                    ld m2 = (low + 2 * high) / 3;
                    if (value(x, m1) < value(x, m2)) {
                        high = m2;
                    } else {
                        low = m1;
                    }
                }
                return value(x, (low + high) * 0.5L);
            };

            ld low = 0;
            ld high = 1;
            for (int it = 0; it < 80; ++it) {
                ld m1 = (2 * low + high) / 3;
                ld m2 = (low + 2 * high) / 3;
                if (best_for_x(m1) < best_for_x(m2)) {
                    high = m2;
                } else {
                    low = m1;
                }
            }
            answer = min(answer, best_for_x((low + high) * 0.5L));
        }
    }

    cout << fixed << setprecision(9) << static_cast<double>(answer) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
