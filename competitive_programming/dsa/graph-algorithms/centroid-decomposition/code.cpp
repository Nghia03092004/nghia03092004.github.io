struct CentroidDecomposition {
    int n;
    vector<vector<int>> tree;
    vector<int> sub;
    vector<int> parent;
    vector<int> depth;
    vector<int> removed;

    explicit CentroidDecomposition(int n)
        : n(n), tree(n), sub(n, 0), parent(n, -1), depth(n, 0), removed(n, 0) {}

    void add_edge(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void dfs_size(int v, int p) {
        sub[v] = 1;
        for (int to : tree[v]) {
            if (to == p || removed[to]) continue;
            dfs_size(to, v);
            sub[v] += sub[to];
        }
    }

    int find_centroid(int v, int p, int total) {
        for (int to : tree[v]) {
            if (to == p || removed[to]) continue;
            if (sub[to] > total / 2) {
                return find_centroid(to, v, total);
            }
        }
        return v;
    }

    void build(int entry = 0, int p = -1, int d = 0) {
        dfs_size(entry, -1);
        int c = find_centroid(entry, -1, sub[entry]);
        removed[c] = 1;
        parent[c] = p;
        depth[c] = d;
        for (int to : tree[c]) {
            if (!removed[to]) {
                build(to, c, d + 1);
            }
        }
    }
};
