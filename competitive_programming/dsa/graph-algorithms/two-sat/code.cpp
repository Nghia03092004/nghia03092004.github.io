struct TwoSat {
    int n;
    vector<vector<int>> graph;
    vector<vector<int>> reverse_graph;
    vector<int> comp;
    vector<int> order;
    vector<int> assignment;

    explicit TwoSat(int n) : n(n), graph(2 * n), reverse_graph(2 * n), comp(2 * n, -1), assignment(n, 0) {}

    int var(int x, bool is_true) const {
        return 2 * x ^ (!is_true);
    }

    void add_implication(int u, int v) {
        graph[u].push_back(v);
        reverse_graph[v].push_back(u);
    }

    void add_or(int x, bool x_true, int y, bool y_true) {
        int a = var(x, x_true);
        int na = var(x, !x_true);
        int b = var(y, y_true);
        int nb = var(y, !y_true);
        add_implication(na, b);
        add_implication(nb, a);
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
        comp[v] = id;
        for (int to : reverse_graph[v]) {
            if (comp[to] == -1) {
                dfs2(to, id);
            }
        }
    }

    bool satisfiable() {
        vector<int> seen(2 * n, 0);
        order.clear();
        fill(comp.begin(), comp.end(), -1);

        for (int v = 0; v < 2 * n; ++v) {
            if (!seen[v]) {
                dfs1(v, seen);
            }
        }
        reverse(order.begin(), order.end());

        int id = 0;
        for (int v : order) {
            if (comp[v] == -1) {
                dfs2(v, id++);
            }
        }

        for (int x = 0; x < n; ++x) {
            if (comp[var(x, true)] == comp[var(x, false)]) {
                return false;
            }
            assignment[x] = comp[var(x, true)] > comp[var(x, false)];
        }
        return true;
    }
};
