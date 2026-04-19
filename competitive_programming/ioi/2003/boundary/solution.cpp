// IOI 2003 - Boundary (Convex Hull)
// Given N points, compute the convex hull using Andrew's Monotone Chain.
// Output: hull vertices, perimeter, area, and count of points on boundary.
// Complexity: O(N log N) for sorting + hull, O(N*H) for boundary counting.

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Point {
    ll x, y;
    Point(ll x = 0, ll y = 0) : x(x), y(y) {}
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    ll cross(const Point& o) const { return x * o.y - y * o.x; }
    bool operator<(const Point& o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
    bool operator==(const Point& o) const {
        return x == o.x && y == o.y;
    }
};

// Andrew's Monotone Chain: returns convex hull vertices in CCW order.
// If includeBoundary is true, collinear points on edges are kept.
vector<Point> convexHull(vector<Point> pts, bool includeBoundary = false) {
    int n = (int)pts.size();
    if (n < 2) return pts;

    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = (int)pts.size();
    if (n < 2) return pts;

    // For strict hull: remove collinear (cross <= 0).
    // For boundary hull: only remove clockwise (cross < 0).
    auto shouldRemove = [&](ll crossVal) -> bool {
        return includeBoundary ? (crossVal < 0) : (crossVal <= 0);
    };

    vector<Point> hull;

    // Lower hull
    for (int i = 0; i < n; i++) {
        while ((int)hull.size() >= 2) {
            Point a = hull[hull.size() - 2], b = hull[hull.size() - 1];
            if (shouldRemove((b - a).cross(pts[i] - a)))
                hull.pop_back();
            else
                break;
        }
        hull.push_back(pts[i]);
    }

    // Upper hull
    int lower_size = (int)hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() > lower_size) {
            Point a = hull[hull.size() - 2], b = hull[hull.size() - 1];
            if (shouldRemove((b - a).cross(pts[i] - a)))
                hull.pop_back();
            else
                break;
        }
        hull.push_back(pts[i]);
    }

    hull.pop_back(); // remove duplicate of first point
    return hull;
}

double perimeter(const vector<Point>& hull) {
    double peri = 0;
    int n = (int)hull.size();
    for (int i = 0; i < n; i++) {
        Point d = hull[(i + 1) % n] - hull[i];
        peri += sqrt((double)(d.x * d.x + d.y * d.y));
    }
    return peri;
}

double area(const vector<Point>& hull) {
    ll a = 0;
    int n = (int)hull.size();
    for (int i = 0; i < n; i++) {
        a += hull[i].cross(hull[(i + 1) % n]);
    }
    return abs(a) / 2.0;
}

// Count how many points from allPts lie on the convex hull boundary
int countOnBoundary(const vector<Point>& hull, const vector<Point>& allPts) {
    int cnt = 0;
    int n = (int)hull.size();
    for (const auto& p : allPts) {
        for (int i = 0; i < n; i++) {
            Point a = hull[i], b = hull[(i + 1) % n];
            Point ab = b - a, ap = p - a;
            if (ab.cross(ap) == 0 &&
                min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
                min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y)) {
                cnt++;
                break;
            }
        }
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // Edge case
    if (N == 0) {
        cout << 0 << "\n";
        return 0;
    }

    vector<Point> pts(N);
    for (int i = 0; i < N; i++) {
        cin >> pts[i].x >> pts[i].y;
    }

    vector<Point> hull = convexHull(pts, false);

    cout << hull.size() << "\n";
    for (const auto& p : hull) {
        cout << p.x << " " << p.y << "\n";
    }

    cout << fixed << setprecision(2) << perimeter(hull) << "\n";
    cout << fixed << setprecision(1) << area(hull) << "\n";
    cout << countOnBoundary(hull, pts) << "\n";

    return 0;
}
