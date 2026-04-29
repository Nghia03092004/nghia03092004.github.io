struct Point {
    long long x;
    long long y;
};

Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}

long long cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

long long cross(Point a, Point b, Point c) {
    return cross(b - a, c - a);
}

int orient(Point a, Point b, Point c) {
    long long value = cross(a, b, c);
    if (value > 0) return 1;
    if (value < 0) return -1;
    return 0;
}

bool on_segment(Point a, Point b, Point p) {
    if (orient(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
