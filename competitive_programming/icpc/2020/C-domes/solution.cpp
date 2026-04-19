#include <bits/stdc++.h>
using namespace std;

namespace {

const long double EPS = 1e-12L;

struct Point {
    long double x = 0;
    long double y = 0;
};

long double cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

Point intersection(const Point& p, const Point& q, const Point& a, const Point& b) {
    long double d1 = cross(a, b, p);
    long double d2 = cross(a, b, q);
    long double t = d1 / (d1 - d2);
    Point result;
    result.x = p.x + (q.x - p.x) * t;
    result.y = p.y + (q.y - p.y) * t;
    return result;
}

bool inside(const Point& p, const Point& a, const Point& b) {
    return cross(a, b, p) <= EPS;
}

bool same_point(const Point& a, const Point& b) {
    return fabsl(a.x - b.x) <= 1e-10L && fabsl(a.y - b.y) <= 1e-10L;
}

vector<Point> clip_polygon(const vector<Point>& polygon, const Point& a, const Point& b) {
    if (polygon.empty()) {
        return {};
    }

    vector<Point> result;
    int m = int(polygon.size());
    for (int i = 0; i < m; ++i) {
        Point current = polygon[i];
        Point next = polygon[(i + 1) % m];
        bool current_inside = inside(current, a, b);
        bool next_inside = inside(next, a, b);

        if (current_inside && next_inside) {
            result.push_back(next);
        } else if (current_inside && !next_inside) {
            result.push_back(intersection(current, next, a, b));
        } else if (!current_inside && next_inside) {
            result.push_back(intersection(current, next, a, b));
            result.push_back(next);
        }
    }

    vector<Point> cleaned;
    for (const Point& p : result) {
        if (cleaned.empty() || !same_point(cleaned.back(), p)) {
            cleaned.push_back(p);
        }
    }
    if (!cleaned.empty() && same_point(cleaned.front(), cleaned.back())) {
        cleaned.pop_back();
    }
    return cleaned;
}

long double polygon_area(const vector<Point>& polygon) {
    if (polygon.size() < 3) {
        return 0;
    }

    long double twice_area = 0;
    int m = int(polygon.size());
    for (int i = 0; i < m; ++i) {
        const Point& current = polygon[i];
        const Point& next = polygon[(i + 1) % m];
        twice_area += current.x * next.y - current.y * next.x;
    }
    return fabsl(twice_area) / 2;
}

void solve() {
    int dx, dy, n;
    cin >> dx >> dy >> n;

    vector<Point> domes(n);
    for (int i = 0; i < n; ++i) {
        cin >> domes[i].x >> domes[i].y;
    }

    vector<int> order(n);
    for (int i = 0; i < n; ++i) {
        cin >> order[i];
        --order[i];
    }

    vector<Point> polygon;
    polygon.push_back({0, 0});
    polygon.push_back({(long double)dx, 0});
    polygon.push_back({(long double)dx, (long double)dy});
    polygon.push_back({0, (long double)dy});

    for (int i = 0; i < n && !polygon.empty(); ++i) {
        for (int j = i + 1; j < n && !polygon.empty(); ++j) {
            polygon = clip_polygon(polygon, domes[order[i]], domes[order[j]]);
        }
    }

    cout << fixed << setprecision(9) << (double)polygon_area(polygon) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
