struct Point {
    long long x;
    long long y;
};

long long cross(Point a, Point b, Point c) {
    long long x1 = b.x - a.x;
    long long y1 = b.y - a.y;
    long long x2 = c.x - a.x;
    long long y2 = c.y - a.y;
    return x1 * y2 - y1 * x2;
}

bool on_segment(Point a, Point b, Point p) {
    if (cross(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

bool segments_intersect(Point a, Point b, Point c, Point d) {
    long long ab_c = cross(a, b, c);
    long long ab_d = cross(a, b, d);
    long long cd_a = cross(c, d, a);
    long long cd_b = cross(c, d, b);

    if ((ab_c == 0 && on_segment(a, b, c)) || (ab_d == 0 && on_segment(a, b, d)) ||
        (cd_a == 0 && on_segment(c, d, a)) || (cd_b == 0 && on_segment(c, d, b))) {
        return true;
    }

    return (ab_c > 0) != (ab_d > 0) && (cd_a > 0) != (cd_b > 0);
}

struct PointD {
    long double x;
    long double y;
};

PointD line_intersection(Point a, Point b, Point c, Point d) {
    long double A1 = b.y - a.y;
    long double B1 = a.x - b.x;
    long double C1 = A1 * a.x + B1 * a.y;
    long double A2 = d.y - c.y;
    long double B2 = c.x - d.x;
    long double C2 = A2 * c.x + B2 * c.y;
    long double det = A1 * B2 - A2 * B1;
    return {
        (B2 * C1 - B1 * C2) / det,
        (A1 * C2 - A2 * C1) / det,
    };
}
