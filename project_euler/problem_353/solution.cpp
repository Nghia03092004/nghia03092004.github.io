#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 353: Risky Moon
 *
 * For each prime r <= 14, find all lattice points on the sphere of radius r
 * (i.e., integer (a,b,c) with a^2+b^2+c^2 = r^2).
 * Build a graph and use Dijkstra to find the minimum-risk path from
 * (0,0,r) to (0,0,-r).
 *
 * Risk of a segment with arc angle theta = (theta/2)^2.
 */

int main() {
    vector<int> primes = {2, 3, 5, 7, 11, 13};
    double total_risk = 0.0;

    for (int r : primes) {
        int r2 = r * r;

        // Enumerate all lattice points on sphere of radius r
        vector<tuple<int,int,int>> points;
        for (int a = -r; a <= r; a++) {
            for (int b = -r; b <= r; b++) {
                int rem = r2 - a*a - b*b;
                if (rem < 0) continue;
                int c = (int)round(sqrt((double)rem));
                if (c * c == rem) {
                    points.push_back({a, b, c});
                    if (c > 0) points.push_back({a, b, -c});
                }
            }
        }

        // Remove duplicates
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());

        int n = points.size();

        // Find north pole (0,0,r) and south pole (0,0,-r)
        int north = -1, south = -1;
        for (int i = 0; i < n; i++) {
            auto [a, b, c] = points[i];
            if (a == 0 && b == 0 && c == r) north = i;
            if (a == 0 && b == 0 && c == -r) south = i;
        }

        if (north == -1 || south == -1) {
            // No lattice points at poles -- skip or handle
            // For r prime: r^2 = 0+0+r^2, so poles always exist
            continue;
        }

        // Dijkstra with all-pairs edges
        vector<double> dist(n, 1e18);
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
        dist[north] = 0.0;
        pq.push({0.0, north});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u] + 1e-15) continue;
            if (u == south) break;

            auto [a1, b1, c1] = points[u];
            for (int v = 0; v < n; v++) {
                if (v == u) continue;
                auto [a2, b2, c2] = points[v];
                // cos(theta) = dot / r^2
                double cos_theta = (double)(a1*a2 + b1*b2 + c1*c2) / (double)r2;
                cos_theta = max(-1.0, min(1.0, cos_theta));
                double theta = acos(cos_theta);
                double risk = (theta / 2.0) * (theta / 2.0);
                if (dist[u] + risk < dist[v] - 1e-15) {
                    dist[v] = dist[u] + risk;
                    pq.push({dist[v], v});
                }
            }
        }

        total_risk += dist[south];
    }

    printf("%.10f\n", total_risk);

    return 0;
}
