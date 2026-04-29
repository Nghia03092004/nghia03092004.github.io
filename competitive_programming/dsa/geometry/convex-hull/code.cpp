struct Point {
    long long x;
    long long y;

    bool operator<(const Point& other) const {
        return x == other.x ? y < other.y : x < other.x;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

long long cross(Point a, Point b, Point c) {
    long long x1 = b.x - a.x;
    long long y1 = b.y - a.y;
    long long x2 = c.x - a.x;
    long long y2 = c.y - a.y;
    return x1 * y2 - y1 * x2;
}

vector<Point> convex_hull(vector<Point> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    if ((int)pts.size() <= 1) {
        return pts;
    }

    vector<Point> lower, upper;
    for (Point p : pts) {
        while ((int)lower.size() >= 2 && cross(lower[(int)lower.size() - 2], lower.back(), p) <= 0) {
            lower.pop_back();
        }
        lower.push_back(p);
    }
    for (int i = (int)pts.size() - 1; i >= 0; --i) {
        Point p = pts[i];
        while ((int)upper.size() >= 2 && cross(upper[(int)upper.size() - 2], upper.back(), p) <= 0) {
            upper.pop_back();
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}
