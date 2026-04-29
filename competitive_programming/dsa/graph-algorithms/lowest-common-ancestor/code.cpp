struct LcaBinaryLifting {
    int n;
    int logn;
    vector<int> depth;
    vector<vector<int>> up;

    LcaBinaryLifting(const vector<vector<int>>& tree, int root = 0) {
        n = (int)tree.size();
        logn = 1;
        while ((1 << logn) <= n) {
            ++logn;
        }
        depth.assign(n, 0);
        up.assign(logn, vector<int>(n, root));

        queue<int> q;
        vector<int> parent(n, root);
        vector<int> seen(n, 0);
        seen[root] = 1;
        q.push(root);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            up[0][v] = parent[v];
            for (int to : tree[v]) {
                if (seen[to]) continue;
                seen[to] = 1;
                parent[to] = v;
                depth[to] = depth[v] + 1;
                q.push(to);
            }
        }

        for (int k = 1; k < logn; ++k) {
            for (int v = 0; v < n; ++v) {
                up[k][v] = up[k - 1][up[k - 1][v]];
            }
        }
    }

    int kth_ancestor(int v, int k) const {
        for (int bit = 0; bit < logn; ++bit) {
            if (k & (1 << bit)) {
                v = up[bit][v];
            }
        }
        return v;
    }

    int lca(int a, int b) const {
        if (depth[a] < depth[b]) {
            swap(a, b);
        }
        a = kth_ancestor(a, depth[a] - depth[b]);
        if (a == b) {
            return a;
        }
        for (int k = logn - 1; k >= 0; --k) {
            if (up[k][a] != up[k][b]) {
                a = up[k][a];
                b = up[k][b];
            }
        }
        return up[0][a];
    }

    int distance(int a, int b) const {
        int c = lca(a, b);
        return depth[a] + depth[b] - 2 * depth[c];
    }
};
