// IOI 1992 - Problem 1: Points (Convex Hull)
// Andrew's monotone chain algorithm. O(n log n)
// Outputs hull vertices in CCW order and computes area via Shoelace formula.
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Point {
    ll x, y;
    bool operator<(const Point& o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
    bool operator==(const Point& o) const {
        return x == o.x && y == o.y;
    }
};

ll cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

vector<Point> convexHull(vector<Point> pts) {
    int n = (int)pts.size();
    if (n < 2) return pts;
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = (int)pts.size();
    if (n < 2) return pts;

    vector<Point> hull;

    // Lower hull
    for (int i = 0; i < n; i++) {
        while ((int)hull.size() >= 2 &&
               cross(hull[hull.size()-2], hull[hull.size()-1], pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }

    // Upper hull
    int lower_size = (int)hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() > lower_size &&
               cross(hull[hull.size()-2], hull[hull.size()-1], pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }

    hull.pop_back(); // remove duplicate of first point
    return hull;
}

int main() {
    int n;
    scanf("%d", &n);

    vector<Point> pts(n);
    for (int i = 0; i < n; i++)
        scanf("%lld%lld", &pts[i].x, &pts[i].y);

    vector<Point> hull = convexHull(pts);

    printf("%d\n", (int)hull.size());
    for (auto& p : hull)
        printf("%lld %lld\n", p.x, p.y);

    // Compute area via Shoelace formula (2 * area to stay integer)
    ll area2 = 0;
    int m = (int)hull.size();
    for (int i = 0; i < m; i++) {
        int j = (i + 1) % m;
        area2 += hull[i].x * hull[j].y;
        area2 -= hull[j].x * hull[i].y;
    }
    if (area2 < 0) area2 = -area2;

    printf("Area = %lld", area2 / 2);
    if (area2 % 2) printf(".5");
    printf("\n");
    return 0;
}
