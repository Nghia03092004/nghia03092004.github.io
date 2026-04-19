#include <algorithm>
#include <cstdint>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

namespace {

struct Point {
    long long x;
    long long y;
};

Point operator+(const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y};
}

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

Point operator-(const Point& a) {
    return {-a.x, -a.y};
}

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator<(const Point& a, const Point& b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

long long dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

long long cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

Point rotate_left(const Point& a) {
    return {-a.y, a.x};
}

long long norm2(const Point& a) {
    return dot(a, a);
}

struct Node {
    bool is_leaf = false;
    Point value = {0, 0};
    vector<int> children;
};

vector<Node> nodes;

struct Direction {
    Point primary;
    Point secondary;
};

bool better_in_direction(const Point& a, const Point& b, const Direction& dir) {
    long long da = dot(dir.primary, a);
    long long db = dot(dir.primary, b);
    if (da != db) {
        return da > db;
    }
    long long sa = dot(dir.secondary, a);
    long long sb = dot(dir.secondary, b);
    if (sa != sb) {
        return sa > sb;
    }
    if (a.x != b.x) {
        return a.x > b.x;
    }
    return a.y > b.y;
}

Point extreme_point(int u, const Direction& dir) {
    const Node& node = nodes[u];
    if (node.is_leaf) {
        return node.value;
    }

    Direction opposite = {{-dir.primary.x, -dir.primary.y},
                          {-dir.secondary.x, -dir.secondary.y}};

    vector<Point> best(node.children.size());
    vector<Point> worst(node.children.size());
    Point total = {0, 0};

    for (size_t i = 0; i < node.children.size(); ++i) {
        int v = node.children[i];
        best[i] = extreme_point(v, dir);
        worst[i] = extreme_point(v, opposite);
        total = total - worst[i];
    }

    int winner = 0;
    Point best_bonus = best[0] + worst[0];
    for (size_t i = 1; i < node.children.size(); ++i) {
        Point candidate_bonus = best[i] + worst[i];
        if (better_in_direction(candidate_bonus, best_bonus, dir)) {
            best_bonus = candidate_bonus;
            winner = static_cast<int>(i);
        }
    }

    return total + best[winner] + worst[winner];
}

void build_hull_arc(const Point& a, const Point& b, set<Point>& hull_points) {
    if (a == b) {
        return;
    }

    Point edge = b - a;
    Direction dir = {rotate_left(edge), edge};
    Point c = extreme_point(1, dir);

    if (c == a || c == b) {
        return;
    }
    if (cross(edge, c - a) <= 0) {
        return;
    }

    hull_points.insert(c);
    build_hull_arc(a, c, hull_points);
    build_hull_arc(c, b, hull_points);
}

void solve() {
    int n;
    cin >> n;
    nodes.assign(n + 1, Node());

    for (int i = 1; i <= n; ++i) {
        int k;
        cin >> k;
        if (k == 0) {
            nodes[i].is_leaf = true;
            cin >> nodes[i].value.x >> nodes[i].value.y;
        } else {
            nodes[i].children.resize(k);
            for (int j = 0; j < k; ++j) {
                cin >> nodes[i].children[j];
            }
        }
    }

    Point start_a = extreme_point(1, {{1, 0}, {0, 1}});
    Point start_b = extreme_point(1, {{-1, 0}, {0, -1}});

    set<Point> hull_points;
    hull_points.insert(start_a);
    hull_points.insert(start_b);
    build_hull_arc(start_a, start_b, hull_points);
    build_hull_arc(start_b, start_a, hull_points);

    long long answer = 0;
    for (set<Point>::const_iterator it = hull_points.begin(); it != hull_points.end(); ++it) {
        answer = max(answer, norm2(*it));
    }

    cout << answer << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
