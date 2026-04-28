struct Line {
    long long m, b;

    __int128 eval128(long long x) const {
        return (__int128)m * x + b;
    }

    long long eval(long long x) const {
        return (long long)eval128(x);
    }
};

struct MonotoneConvexHullTrick {
    deque<Line> hull;

    static bool is_bad(const Line& a, const Line& b, const Line& c) {
        return (__int128)(b.b - a.b) * (a.m - c.m) >= (__int128)(c.b - a.b) * (a.m - b.m);
    }

    void add_line(long long m, long long b) {
        Line line{m, b};
        while (!hull.empty() && hull.back().m == line.m) {
            if (hull.back().b <= line.b) return;
            hull.pop_back();
        }
        while (hull.size() >= 2 && is_bad(hull[hull.size() - 2], hull.back(), line)) {
            hull.pop_back();
        }
        hull.push_back(line);
    }

    long long query(long long x) {
        while (hull.size() >= 2 && hull[0].eval128(x) >= hull[1].eval128(x)) {
            hull.pop_front();
        }
        return hull.front().eval(x);
    }
};
