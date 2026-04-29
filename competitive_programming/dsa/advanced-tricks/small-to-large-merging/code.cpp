struct SmallToLargeDistinctColors {
    vector<vector<int>> tree;
    vector<int> color;
    vector<int> answer;
    vector<map<int, int>> bag;

    explicit SmallToLargeDistinctColors(int n)
        : tree(n), color(n), answer(n, 0), bag(n) {}

    void add_edge(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void dfs(int v, int parent = -1) {
        bag[v][color[v]] = 1;
        for (int to : tree[v]) {
            if (to == parent) continue;
            dfs(to, v);
            if (bag[to].size() > bag[v].size()) {
                swap(bag[to], bag[v]);
            }
            for (auto [key, freq] : bag[to]) {
                bag[v][key] += freq;
            }
        }
        answer[v] = (int)bag[v].size();
    }
};
