#include <bits/stdc++.h>
using namespace std;

namespace {

const long double PI = acosl(-1.0L);
const long double TAU = 2.0L * PI;
const long double EPS = 1e-18L;

struct Point {
    long double x;
    long double y;
};

struct Event {
    long double angle;
    int delta;
};

void add_interval(vector<Event>& events, long double left, long double right) {
    while (left < 0) {
        left += TAU;
        right += TAU;
    }
    while (left >= TAU) {
        left -= TAU;
        right -= TAU;
    }

    if (right < TAU + EPS) {
        events.push_back({left, +1});
        events.push_back({right, -1});
    } else {
        events.push_back({left, +1});
        events.push_back({TAU, -1});
        events.push_back({0, +1});
        events.push_back({right - TAU, -1});
    }
}

void solve() {
    int n;
    long double t;
    cin >> n >> t;

    vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
    }

    int answer = 2;
    vector<Event> events;

    for (int i = 0; i < n; ++i) {
        events.clear();
        events.reserve(4 * (n - 1) + 4);

        for (int j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }

            long double dx = points[j].x - points[i].x;
            long double dy = points[j].y - points[i].y;
            long double dist = hypotl(dx, dy);
            long double angle = atan2l(dy, dx);
            if (angle < 0) {
                angle += TAU;
            }

            if (dist <= t + EPS) {
                add_interval(events, angle - PI, angle);
            } else {
                long double delta = asinl(t / dist);
                add_interval(events, angle - delta, angle);
                add_interval(events, angle - PI, angle - PI + delta);
            }
        }

        sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
            if (fabsl(a.angle - b.angle) > EPS) {
                return a.angle < b.angle;
            }
            return a.delta > b.delta;
        });

        int current = 1;
        for (const Event& event : events) {
            current += event.delta;
            answer = max(answer, current);
        }
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
