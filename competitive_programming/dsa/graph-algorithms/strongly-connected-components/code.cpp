struct StronglyConnectedComponents {
    int n;
    vector<vector<int>> graph;
    vector<vector<int>> reverse_graph;
    vector<int> order;
    vector<int> comp_id;
    vector<vector<int>> components;

    explicit StronglyConnectedComponents(int n)
        : n(n), graph(n), reverse_graph(n), comp_id(n, -1) {}

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        reverse_graph[v].push_back(u);
    }

    void dfs1(int v, vector<int>& seen) {
        seen[v] = 1;
        for (int to : graph[v]) {
            if (!seen[to]) {
                dfs1(to, seen);
            }
        }
        order.push_back(v);
    }

    void dfs2(int v, int id) {
        comp_id[v] = id;
        components.back().push_back(v);
        for (int to : reverse_graph[v]) {
            if (comp_id[to] == -1) {
                dfs2(to, id);
            }
        }
    }

    void build() {
        vector<int> seen(n, 0);
        for (int v = 0; v < n; ++v) {
            if (!seen[v]) {
                dfs1(v, seen);
            }
        }
        reverse(order.begin(), order.end());
        for (int v : order) {
            if (comp_id[v] == -1) {
                components.push_back({});
                dfs2(v, (int)components.size() - 1);
            }
        }
    }
};
