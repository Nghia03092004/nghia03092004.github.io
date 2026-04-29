struct BridgesAndArticulationPoints {
    int n;
    vector<vector<int>> graph;
    vector<int> tin;
    vector<int> low;
    vector<int> seen;
    vector<int> is_articulation;
    vector<pair<int, int>> bridges;
    int timer = 0;

    explicit BridgesAndArticulationPoints(int n)
        : n(n), graph(n), tin(n, -1), low(n, -1), seen(n, 0), is_articulation(n, 0) {}

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    void dfs(int v, int parent = -1) {
        seen[v] = 1;
        tin[v] = low[v] = timer++;
        int child_count = 0;

        for (int to : graph[v]) {
            if (to == parent) continue;
            if (seen[to]) {
                low[v] = min(low[v], tin[to]);
            } else {
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                if (low[to] > tin[v]) {
                    bridges.push_back({v, to});
                }
                if (parent != -1 && low[to] >= tin[v]) {
                    is_articulation[v] = 1;
                }
                ++child_count;
            }
        }

        if (parent == -1 && child_count > 1) {
            is_articulation[v] = 1;
        }
    }

    void build() {
        for (int v = 0; v < n; ++v) {
            if (!seen[v]) {
                dfs(v);
            }
        }
    }
};
