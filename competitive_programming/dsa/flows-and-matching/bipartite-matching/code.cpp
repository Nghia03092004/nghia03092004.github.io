struct HopcroftKarp {
    int n_left;
    int n_right;
    vector<vector<int>> graph;
    vector<int> dist;
    vector<int> match_left;
    vector<int> match_right;

    HopcroftKarp(int n_left, int n_right)
        : n_left(n_left),
          n_right(n_right),
          graph(n_left),
          dist(n_left),
          match_left(n_left, -1),
          match_right(n_right, -1) {}

    void add_edge(int u, int v) {
        graph[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        fill(dist.begin(), dist.end(), -1);
        for (int u = 0; u < n_left; ++u) {
            if (match_left[u] == -1) {
                dist[u] = 0;
                q.push(u);
            }
        }

        bool found = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : graph[u]) {
                int next = match_right[v];
                if (next == -1) {
                    found = true;
                } else if (dist[next] == -1) {
                    dist[next] = dist[u] + 1;
                    q.push(next);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : graph[u]) {
            int next = match_right[v];
            if (next == -1 || (dist[next] == dist[u] + 1 && dfs(next))) {
                match_left[u] = v;
                match_right[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int maximum_matching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 0; u < n_left; ++u) {
                if (match_left[u] == -1 && dfs(u)) {
                    ++matching;
                }
            }
        }
        return matching;
    }
};
