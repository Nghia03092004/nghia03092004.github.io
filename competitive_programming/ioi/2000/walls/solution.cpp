// IOI 2000 - Walls
// Shortest path between points avoiding convex polygonal walls.
// Uses visibility graph + Dijkstra's algorithm.
// Nodes: polygon vertices + source + destination.
// Edges: pairs of mutually visible nodes weighted by Euclidean distance.
// Complexity: O(V^2 * P + V^2 log V) per query.

#include <bits/stdc++.h>
using namespace std;

typedef double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    Point operator+(const Point& o) const { return {x + o.x, y + o.y}; }
    ld cross(const Point& o) const { return x * o.y - y * o.x; }
    ld dot(const Point& o) const { return x * o.x + y * o.y; }
    ld norm() const { return sqrt(x * x + y * y); }
};

ld dist(Point a, Point b) { return (a - b).norm(); }

// Check if segments AB and CD have a proper interior intersection
bool segmentsIntersectProperly(Point a, Point b, Point c, Point d) {
    ld d1 = (b - a).cross(c - a);
    ld d2 = (b - a).cross(d - a);
    ld d3 = (d - c).cross(a - c);
    ld d4 = (d - c).cross(b - c);

    if (((d1 > EPS && d2 < -EPS) || (d1 < -EPS && d2 > EPS)) &&
        ((d3 > EPS && d4 < -EPS) || (d3 < -EPS && d4 > EPS)))
        return true;
    return false;
}

// Check if segment PQ is blocked by a convex polygon
bool segmentBlockedByPolygon(Point p, Point q, vector<Point>& poly) {
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if (segmentsIntersectProperly(p, q, a, b)) {
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; // number of walls (polygons)
    cin >> N;

    vector<vector<Point>> polygons(N);
    vector<Point> allPoints;

    for (int i = 0; i < N; i++) {
        int m;
        cin >> m;
        polygons[i].resize(m);
        for (int j = 0; j < m; j++) {
            cin >> polygons[i][j].x >> polygons[i][j].y;
            allPoints.push_back(polygons[i][j]);
        }
    }

    int Q; // number of queries
    cin >> Q;

    while (Q--) {
        Point src, dst;
        cin >> src.x >> src.y >> dst.x >> dst.y;

        // Build visibility graph: polygon vertices + src + dst
        vector<Point> nodes = allPoints;
        int srcIdx = (int)nodes.size();
        nodes.push_back(src);
        int dstIdx = (int)nodes.size();
        nodes.push_back(dst);

        int V = (int)nodes.size();
        vector<vector<pair<int, ld>>> adj(V);

        // Check visibility between all pairs of nodes
        for (int i = 0; i < V; i++) {
            for (int j = i + 1; j < V; j++) {
                bool blocked = false;
                for (int k = 0; k < N; k++) {
                    if (segmentBlockedByPolygon(nodes[i], nodes[j], polygons[k])) {
                        blocked = true;
                        break;
                    }
                }
                if (!blocked) {
                    ld d = dist(nodes[i], nodes[j]);
                    adj[i].push_back({j, d});
                    adj[j].push_back({i, d});
                }
            }
        }

        // Dijkstra from src to dst
        vector<ld> dists(V, 1e18);
        priority_queue<pair<ld, int>, vector<pair<ld, int>>, greater<>> pq;
        dists[srcIdx] = 0;
        pq.push({0, srcIdx});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dists[u] + EPS) continue;
            for (auto [v, w] : adj[u]) {
                if (dists[u] + w < dists[v] - EPS) {
                    dists[v] = dists[u] + w;
                    pq.push({dists[v], v});
                }
            }
        }

        cout << fixed << setprecision(2) << dists[dstIdx] << "\n";
    }

    return 0;
}
