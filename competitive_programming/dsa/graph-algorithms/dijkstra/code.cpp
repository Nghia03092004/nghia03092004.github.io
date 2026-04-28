struct Dijkstra {
    static constexpr long long INF = (1LL << 62);

    int n;
    vector<vector<pair<int, int>>> adj;

    Dijkstra() : n(0) {}
    explicit Dijkstra(int n) { init(n); }

    void init(int n_) {
        n = n_;
        adj.assign(n, {});
    }

    void add_edge(int u, int v, int w, bool undirected = false) {
        adj[u].push_back({v, w});
        if (undirected) adj[v].push_back({u, w});
    }

    pair<vector<long long>, vector<int>> shortest_paths(int source) const {
        vector<long long> dist(n, INF);
        vector<int> parent(n, -1);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [du, u] = pq.top();
            pq.pop();
            if (du != dist[u]) continue;

            for (auto [v, w] : adj[u]) {
                if (dist[v] > du + w) {
                    dist[v] = du + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        return {dist, parent};
    }

    vector<int> restore_path(int source, int target, const vector<int>& parent) const {
        vector<int> path;
        for (int v = target; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());
        if (path.empty() || path[0] != source) return {};
        return path;
    }
};
