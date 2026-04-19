// IOI 1996 - Network of Schools
// Kosaraju's SCC + condensed DAG analysis
// Task A: count sources (in-degree 0) in condensed DAG
// Task B: max(sources, sinks) to make graph strongly connected
// Time: O(n + m), Space: O(n + m)
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 105;
int n;
vector<int> adj[MAXN], radj[MAXN];
int comp[MAXN];
bool visited[MAXN];
int order_arr[MAXN], order_cnt;

void dfs1(int u) {
    visited[u] = true;
    for (int v : adj[u])
        if (!visited[v]) dfs1(v);
    order_arr[order_cnt++] = u;
}

void dfs2(int u, int c) {
    comp[u] = c;
    for (int v : radj[u])
        if (comp[v] == -1) dfs2(v, c);
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int v;
        while (scanf("%d", &v) == 1 && v != 0) {
            adj[i].push_back(v);
            radj[v].push_back(i);
        }
    }

    // Kosaraju's SCC
    memset(visited, false, sizeof(visited));
    order_cnt = 0;
    for (int i = 1; i <= n; i++)
        if (!visited[i]) dfs1(i);

    memset(comp, -1, sizeof(comp));
    int numSCC = 0;
    for (int i = order_cnt - 1; i >= 0; i--)
        if (comp[order_arr[i]] == -1)
            dfs2(order_arr[i], numSCC++);

    if (numSCC == 1) {
        printf("1\n0\n");
        return 0;
    }

    // Build condensed DAG, track which SCC pairs have edges
    set<pair<int,int>> condEdges;
    bool hasIn[MAXN] = {}, hasOut[MAXN] = {};
    for (int u = 1; u <= n; u++)
        for (int v : adj[u])
            if (comp[u] != comp[v]) {
                auto e = make_pair(comp[u], comp[v]);
                if (condEdges.insert(e).second) {
                    hasIn[comp[v]] = true;
                    hasOut[comp[u]] = true;
                }
            }

    int sources = 0, sinks = 0;
    for (int i = 0; i < numSCC; i++) {
        if (!hasIn[i]) sources++;
        if (!hasOut[i]) sinks++;
    }

    printf("%d\n%d\n", sources, max(sources, sinks));
    return 0;
}
