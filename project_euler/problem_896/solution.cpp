#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 896: Divisible Ranges
 *
 * A range [a, a+n-1] of length n is a "divisible range" if the numbers can
 * be arranged so that position k gets a multiple of k (for k=1..n).
 *
 * Find the 36th divisible range of length 36.
 *
 * Key insight: Use Hopcroft-Karp bipartite matching with early termination.
 * Also, we can quickly reject ranges where some position k has no valid value.
 */

struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> matchL, matchR, dist;

    HopcroftKarp(int n, int m) : n(n), m(m), adj(n), matchL(n, -1), matchR(m, -1), dist(n) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; u++) {
            if (matchL[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INT_MAX;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int w = matchR[v];
                if (w == -1) {
                    found = true;
                } else if (dist[w] == INT_MAX) {
                    dist[w] = dist[u] + 1;
                    q.push(w);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            int w = matchR[v];
            if (w == -1 || (dist[w] == dist[u] + 1 && dfs(w))) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        dist[u] = INT_MAX;
        return false;
    }

    int maxMatching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 0; u < n; u++) {
                if (matchL[u] == -1) {
                    if (dfs(u)) matching++;
                }
            }
        }
        return matching;
    }
};

bool isDivisibleRange(long long a, int n) {
    // Quick check: for each position k, count how many values in [a, a+n-1] are divisible by k
    // If any position has 0 candidates, return false immediately
    for (int k = 1; k <= n; k++) {
        long long first_mult = ((a + k - 1) / k) * k;
        if (first_mult > a + n - 1) return false;
    }

    HopcroftKarp hk(n, n);
    for (int k = 1; k <= n; k++) {
        long long first_mult = ((a + k - 1) / k) * k;
        for (long long v = first_mult; v <= a + n - 1; v += k) {
            hk.addEdge(k - 1, (int)(v - a));
        }
    }
    return hk.maxMatching() == n;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 36;
    int target = 36;
    int count = 0;

    // Optimization: the largest position k=n=36 needs at least one multiple in [a, a+35].
    // A multiple of 36 occurs every 36 numbers, so [a, a+35] always contains at least one multiple of 36.
    // Similarly for other large k. The bottleneck positions are those with few candidates.

    for (long long a = 1; ; a++) {
        if (isDivisibleRange(a, n)) {
            count++;
            // Print progress
            // fprintf(stderr, "Found divisible range #%d starting at %lld\n", count, a);
            if (count == target) {
                cout << a << endl;
                return 0;
            }
        }
    }

    return 0;
}
