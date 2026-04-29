struct TreeIndependentSet {
    vector<vector<int>> tree;
    vector<long long> take;
    vector<long long> skip;

    explicit TreeIndependentSet(int n) : tree(n), take(n, 0), skip(n, 0) {}

    void add_edge(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void dfs(int v, int parent = -1) {
        take[v] = 1;
        skip[v] = 0;
        for (int to : tree[v]) {
            if (to == parent) continue;
            dfs(to, v);
            take[v] += skip[to];
            skip[v] += max(take[to], skip[to]);
        }
    }

    long long solve(int root = 0) {
        dfs(root);
        return max(take[root], skip[root]);
    }
};
