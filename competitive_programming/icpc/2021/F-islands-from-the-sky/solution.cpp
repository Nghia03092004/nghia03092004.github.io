#include <bits/stdc++.h>
using namespace std;

namespace {

struct Point {
    long double x;
    long double y;
};

struct Flight {
    long double x1;
    long double y1;
    long double z1;
    long double x2;
    long double y2;
    long double z2;
};

const long double INF = 1e100L;
const long double EPS = 1e-12L;
const long double PI = acosl(-1.0L);

long double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

long double required_tan(const vector<Point>& island, const Flight& flight) {
    Point start = {flight.x1, flight.y1};
    Point end = {flight.x2, flight.y2};
    Point dir = {end.x - start.x, end.y - start.y};
    long double length = sqrtl(dot(dir, dir));

    Point unit = {dir.x / length, dir.y / length};
    Point normal = {-unit.y, unit.x};

    long double need = 0.0L;
    for (const Point& vertex : island) {
        Point rel = {vertex.x - start.x, vertex.y - start.y};
        long double along = dot(rel, unit);
        if (along < -EPS || along > length + EPS) {
            return INF;
        }

        long double t = along / length;
        long double altitude = flight.z1 + (flight.z2 - flight.z1) * t;
        long double offset = fabsl(dot(rel, normal));
        need = max(need, offset / altitude);
    }

    return need;
}

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<Point>> islands(n);
    for (int i = 0; i < n; ++i) {
        int vertex_count;
        cin >> vertex_count;
        islands[i].resize(vertex_count);
        for (int j = 0; j < vertex_count; ++j) {
            cin >> islands[i][j].x >> islands[i][j].y;
        }
    }

    vector<Flight> flights(m);
    for (int i = 0; i < m; ++i) {
        cin >> flights[i].x1 >> flights[i].y1 >> flights[i].z1
            >> flights[i].x2 >> flights[i].y2 >> flights[i].z2;
    }

    long double answer_tan = 0.0L;
    for (const vector<Point>& island : islands) {
        long double best = INF;
        for (const Flight& flight : flights) {
            best = min(best, required_tan(island, flight));
        }
        if (best >= INF / 2) {
            cout << "impossible\n";
            return;
        }
        answer_tan = max(answer_tan, best);
    }

    long double answer = atanl(answer_tan) * 180.0L / PI;
    cout << fixed << setprecision(12) << static_cast<double>(answer) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
