#include <bits/stdc++.h>
using namespace std;

namespace {

struct Edge {
    int to;
    int id;
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int> > edges(m);
    vector<vector<Edge> > graph(n + 1);
    vector<int> degree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        edges[i] = make_pair(u, v);
        graph[u].push_back({v, i});
        graph[v].push_back({u, i});
        ++degree[u];
        ++degree[v];
    }

    vector<int> comp(n + 1, -1);
    vector<int> comp_edge_count;
    vector<vector<int> > comp_vertices;
    int comp_cnt = 0;
    for (int start = 1; start <= n; ++start) {
        if (comp[start] != -1) {
            continue;
        }
        queue<int> q;
        q.push(start);
        comp[start] = comp_cnt;
        comp_vertices.push_back({});
        long long degree_sum = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            comp_vertices.back().push_back(v);
            degree_sum += graph[v].size();
            for (const Edge& e : graph[v]) {
                if (comp[e.to] == -1) {
                    comp[e.to] = comp_cnt;
                    q.push(e.to);
                }
            }
        }
        comp_edge_count.push_back((int)(degree_sum / 2));
        ++comp_cnt;
    }

    queue<int> q;
    vector<char> removed(n + 1, false);
    for (int v = 1; v <= n; ++v) {
        if (degree[v] <= 1) {
            q.push(v);
        }
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (removed[v] || degree[v] > 1) {
            continue;
        }
        removed[v] = true;
        for (const Edge& e : graph[v]) {
            if (removed[e.to]) {
                continue;
            }
            --degree[e.to];
            if (degree[e.to] == 1) {
                q.push(e.to);
            }
        }
    }

    vector<char> comp_has_core(comp_cnt, false);
    for (int v = 1; v <= n; ++v) {
        if (!removed[v]) {
            comp_has_core[comp[v]] = true;
        }
    }

    vector<pair<int, int> > answer;
    for (int id = 0; id < m; ++id) {
        int u = edges[id].first;
        int v = edges[id].second;
        if (removed[u] != removed[v]) {
            if (!removed[u]) {
                answer.push_back(make_pair(u, v));
            } else {
                answer.push_back(make_pair(v, u));
            }
        }
    }

    for (int cid = 0; cid < comp_cnt; ++cid) {
        if (comp_has_core[cid]) {
            continue;
        }
        for (int v : comp_vertices[cid]) {
            if ((int)graph[v].size() == 1) {
                answer.push_back(make_pair(v, graph[v][0].to));
            }
        }
    }

    sort(answer.begin(), answer.end());
    cout << answer.size() << '\n';
    for (const pair<int, int>& sign : answer) {
        cout << sign.first << ' ' << sign.second << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
