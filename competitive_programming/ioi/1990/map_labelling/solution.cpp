// IOI 1990 - Problem 2: Map Labelling
// 2-SAT: each point has 2 candidate label positions, find non-overlapping assignment.
// Uses Kosaraju's SCC algorithm on the implication graph.
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2005;

struct TwoSat {
    int n;
    vector<int> adj[2 * MAXN], radj[2 * MAXN];
    int comp[2 * MAXN];
    bool visited[2 * MAXN];
    vector<int> topo;

    void init(int _n) {
        n = _n;
        for (int i = 0; i < 2 * n; i++) {
            adj[i].clear();
            radj[i].clear();
        }
    }

    // Add clause (a OR b). Literal for variable i: true=2*i, false=2*i+1.
    void addClause(int a, int b) {
        adj[a ^ 1].push_back(b);
        adj[b ^ 1].push_back(a);
        radj[b].push_back(a ^ 1);
        radj[a].push_back(b ^ 1);
    }

    void dfs1(int u) {
        visited[u] = true;
        for (int v : adj[u])
            if (!visited[v]) dfs1(v);
        topo.push_back(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : radj[u])
            if (comp[v] == -1) dfs2(v, c);
    }

    bool solve(vector<bool>& result) {
        fill(visited, visited + 2 * n, false);
        topo.clear();
        for (int i = 0; i < 2 * n; i++)
            if (!visited[i]) dfs1(i);
        fill(comp, comp + 2 * n, -1);
        int c = 0;
        for (int i = 2 * n - 1; i >= 0; i--) {
            int u = topo[i];
            if (comp[u] == -1) dfs2(u, c++);
        }
        result.resize(n);
        for (int i = 0; i < n; i++) {
            if (comp[2 * i] == comp[2 * i + 1]) return false;
            result[i] = (comp[2 * i] > comp[2 * i + 1]);
        }
        return true;
    }
};

struct Point {
    double x, y, w, h;
    // Position 0: label to the right; Position 1: label to the left
    double lx(int pos) const { return pos == 0 ? x : x - w; }
    double ly(int /*pos*/) const { return y; }
};

bool overlaps(const Point& a, int pa, const Point& b, int pb) {
    double ax = a.lx(pa), ay = a.ly(pa);
    double bx = b.lx(pb), by = b.ly(pb);
    return ax < bx + b.w && bx < ax + a.w &&
           ay < by + b.h && by < ay + a.h;
}

int main() {
    int n;
    scanf("%d", &n);

    vector<Point> pts(n);
    for (int i = 0; i < n; i++)
        scanf("%lf%lf%lf%lf", &pts[i].x, &pts[i].y, &pts[i].w, &pts[i].h);

    TwoSat sat;
    sat.init(n);

    // For each pair, if two position choices overlap, add a 2-SAT clause
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int pi = 0; pi < 2; pi++) {
                for (int pj = 0; pj < 2; pj++) {
                    if (overlaps(pts[i], pi, pts[j], pj)) {
                        int li = (pi == 0) ? (2 * i + 1) : (2 * i);
                        int lj = (pj == 0) ? (2 * j + 1) : (2 * j);
                        sat.addClause(li, lj);
                    }
                }
            }
        }
    }

    vector<bool> result;
    if (sat.solve(result)) {
        printf("YES\n");
        for (int i = 0; i < n; i++) {
            int pos = result[i] ? 0 : 1;
            printf("Point %d: position %d (%.2f, %.2f)\n",
                   i + 1, pos, pts[i].lx(pos), pts[i].ly(pos));
        }
    } else {
        printf("NO\n");
    }
    return 0;
}
