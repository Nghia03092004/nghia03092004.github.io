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

int point_in_polygon(const vector<Point>& poly, Point p) {
    bool inside = false;
    int n = (int)poly.size();
    for (int i = 0; i < n; ++i) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        if (on_segment(a, b, p)) {
            return 1;
        }

        bool crosses = (a.y > p.y) != (b.y > p.y);
        if (crosses) {
            long double x = a.x + (long double)(b.x - a.x) * (p.y - a.y) / (b.y - a.y);
            if (x > p.x) {
                inside = !inside;
            }
        }
    }
    return inside ? 2 : 0;
}
