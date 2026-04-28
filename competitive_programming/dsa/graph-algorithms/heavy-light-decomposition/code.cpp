struct SegTree {
    int n;
    vector<long long> tree;

    SegTree() : n(0) {}

    void build(const vector<long long>& values) {
        n = 1;
        while (n < (int)values.size()) n <<= 1;
        tree.assign(2 * n, 0);
        for (int i = 0; i < (int)values.size(); ++i) tree[n + i] = values[i];
        for (int i = n - 1; i > 0; --i) tree[i] = tree[i << 1] + tree[i << 1 | 1];
    }

    void point_set(int pos, long long value) {
        pos += n;
        tree[pos] = value;
        for (pos >>= 1; pos > 0; pos >>= 1) {
            tree[pos] = tree[pos << 1] + tree[pos << 1 | 1];
        }
    }

    long long range_sum(int left, int right) const {
        long long answer = 0;
        for (left += n, right += n; left <= right; left >>= 1, right >>= 1) {
            if (left & 1) answer += tree[left++];
            if (!(right & 1)) answer += tree[right--];
        }
        return answer;
    }
};

struct HeavyLightDecomposition {
    int n, current_pos;
    vector<vector<int>> graph;
    vector<int> parent, depth, heavy, head, pos, subtree;
    vector<long long> values, base;
    SegTree seg;

    HeavyLightDecomposition() : n(0), current_pos(0) {}
    explicit HeavyLightDecomposition(int n) { init(n); }

    void init(int n_) {
        n = n_;
        current_pos = 0;
        graph.assign(n, {});
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, 0);
        pos.assign(n, 0);
        subtree.assign(n, 0);
        values.assign(n, 0);
        base.assign(n, 0);
    }

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int dfs_size(int v, int p) {
        parent[v] = p;
        subtree[v] = 1;
        int best_subtree = 0;

        for (int to : graph[v]) {
            if (to == p) continue;
            depth[to] = depth[v] + 1;
            int child_size = dfs_size(to, v);
            subtree[v] += child_size;
            if (child_size > best_subtree) {
                best_subtree = child_size;
                heavy[v] = to;
            }
        }

        return subtree[v];
    }

    void dfs_decompose(int v, int chain_head) {
        head[v] = chain_head;
        pos[v] = current_pos;
        base[current_pos++] = values[v];

        if (heavy[v] != -1) {
            dfs_decompose(heavy[v], chain_head);
        }

        for (int to : graph[v]) {
            if (to == parent[v] || to == heavy[v]) continue;
            dfs_decompose(to, to);
        }
    }

    void build(const vector<long long>& initial_values, int root = 0) {
        values = initial_values;
        fill(heavy.begin(), heavy.end(), -1);
        depth[root] = 0;
        current_pos = 0;
        dfs_size(root, -1);
        dfs_decompose(root, root);
        seg.build(base);
    }

    void point_set(int v, long long value) {
        seg.point_set(pos[v], value);
    }

    long long query_path(int a, int b) const {
        long long answer = 0;
        while (head[a] != head[b]) {
            if (depth[head[a]] < depth[head[b]]) swap(a, b);
            answer += seg.range_sum(pos[head[a]], pos[a]);
            a = parent[head[a]];
        }
        if (depth[a] > depth[b]) swap(a, b);
        answer += seg.range_sum(pos[a], pos[b]);
        return answer;
    }

    long long query_subtree(int v) const {
        return seg.range_sum(pos[v], pos[v] + subtree[v] - 1);
    }
};
