struct EulerTour {
    vector<vector<int>> tree;
    vector<int> tin;
    vector<int> tout;
    vector<int> order;
    int timer = 0;

    explicit EulerTour(int n) : tree(n), tin(n), tout(n), order(n) {}

    void add_edge(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void dfs(int v, int parent = -1) {
        tin[v] = timer;
        order[timer] = v;
        ++timer;
        for (int to : tree[v]) {
            if (to == parent) continue;
            dfs(to, v);
        }
        tout[v] = timer - 1;
    }

    pair<int, int> subtree_interval(int v) const {
        return {tin[v], tout[v]};
    }
};
