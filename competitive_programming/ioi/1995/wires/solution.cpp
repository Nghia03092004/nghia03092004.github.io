// IOI 1995 - Wires (Minimum Spanning Tree)
// Kruskal's algorithm on complete graph of Euclidean distances
// Time: O(n^2 log n), Space: O(n^2)
#include <bits/stdc++.h>
using namespace std;

struct Edge {
    double w;
    int u, v;
    bool operator<(const Edge& o) const { return w < o.w; }
};

int par[55], rnk[55];

int find(int x) {
    while (par[x] != x) x = par[x] = par[par[x]];
    return x;
}

bool unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return false;
    if (rnk[a] < rnk[b]) swap(a, b);
    par[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
    return true;
}

double px[55], py[55];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%lf %lf", &px[i], &py[i]);

    if (n <= 1) {
        printf("%.2f\n", 0.0);
        return 0;
    }

    // Build all edges
    vector<Edge> edges;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            double dx = px[i] - px[j], dy = py[i] - py[j];
            edges.push_back({sqrt(dx * dx + dy * dy), i, j});
        }

    sort(edges.begin(), edges.end());

    // Kruskal's MST
    for (int i = 0; i < n; i++) { par[i] = i; rnk[i] = 0; }

    double totalLen = 0;
    int edgesUsed = 0;

    for (auto& e : edges) {
        if (unite(e.u, e.v)) {
            totalLen += e.w;
            if (++edgesUsed == n - 1) break;
        }
    }

    printf("%.2f\n", totalLen);
    return 0;
}
