#include <bits/stdc++.h>
using namespace std;

namespace {

using ld = long double;

struct Point {
    ld x = 0;
    ld y = 0;
};

ld cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

ld opponent_score(const vector<long long>& die, long long value) {
    auto lo = lower_bound(die.begin(), die.end(), value);
    auto hi = upper_bound(die.begin(), die.end(), value);
    long long greater = die.end() - hi;
    long long equal = hi - lo;
    return (greater + 0.5L * equal) / static_cast<ld>(die.size());
}

ld duel_score(const vector<long long>& first, const vector<long long>& second) {
    long long wins = 0;
    long long ties = 0;
    for (long long x : first) {
        wins += lower_bound(second.begin(), second.end(), x) - second.begin();
        auto range = equal_range(second.begin(), second.end(), x);
        ties += range.second - range.first;
    }
    ld total = static_cast<ld>(first.size()) * second.size();
    return (wins + 0.5L * ties) / total;
}

void solve() {
    vector<long long> a, b;
    int n;

    cin >> n;
    a.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    cin >> n;
    b.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
    }

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    if (duel_score(a, b) < 0.5L) {
        swap(a, b);
    }

    vector<long long> candidates = {1};
    long long max_value = max(a.back(), b.back());
    candidates.push_back(max_value + 1);
    for (long long x : a) {
        candidates.push_back(x);
        if (x > 1) {
            candidates.push_back(x - 1);
        }
    }
    for (long long x : b) {
        candidates.push_back(x);
        if (x > 1) {
            candidates.push_back(x - 1);
        }
    }
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    vector<Point> points;
    points.reserve(candidates.size());
    for (long long value : candidates) {
        points.push_back({opponent_score(a, value), opponent_score(b, value)});
    }

    sort(points.begin(), points.end(), [](const Point& lhs, const Point& rhs) {
        if (lhs.x != rhs.x) {
            return lhs.x < rhs.x;
        }
        return lhs.y < rhs.y;
    });

    vector<Point> filtered;
    for (const Point& p : points) {
        if (!filtered.empty() && fabsl(filtered.back().x - p.x) <= 1e-18L) {
            filtered.back().y = max(filtered.back().y, p.y);
        } else {
            filtered.push_back(p);
        }
    }

    vector<Point> hull;
    for (const Point& p : filtered) {
        while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull.back(), p) >= -1e-18L) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    ld best_y = -1;
    ld best_x = 2;
    for (int i = 0; i < static_cast<int>(hull.size()); ++i) {
        if (hull[i].x <= 0.5L + 1e-18L) {
            best_y = max(best_y, hull[i].y);
        }
        if (hull[i].y >= 0.5L - 1e-18L) {
            best_x = min(best_x, hull[i].x);
        }
        if (i + 1 == static_cast<int>(hull.size())) {
            continue;
        }

        const Point& p = hull[i];
        const Point& q = hull[i + 1];
        if ((p.x - 0.5L) * (q.x - 0.5L) < -1e-18L) {
            ld ratio = (0.5L - p.x) / (q.x - p.x);
            best_y = max(best_y, p.y + ratio * (q.y - p.y));
        }
        if ((p.y - 0.5L) * (q.y - 0.5L) < -1e-18L) {
            ld ratio = (0.5L - p.y) / (q.y - p.y);
            best_x = min(best_x, p.x + ratio * (q.x - p.x));
        }
    }

    cout << fixed << setprecision(9)
         << static_cast<double>(1.0L - best_y) << ' '
         << static_cast<double>(1.0L - best_x) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
