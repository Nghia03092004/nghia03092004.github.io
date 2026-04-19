#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent;

    void init(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
};

struct KruskalTree {
    int total = 0;
    int root = -1;
    int logn = 0;
    vector<int> value;
    vector<int> parent;
    vector<array<int, 2>> child;
    vector<int> tin, tout;
    vector<vector<int>> up;
    int timer = 0;

    void dfs(int u) {
        tin[u] = timer++;
        for (int v : child[u]) {
            if (v != -1) {
                dfs(v);
            }
        }
        tout[u] = timer - 1;
    }

    void build(int n, vector<pair<int, int>> edges, bool human) {
        total = n;
        value.assign(2 * n, -1);
        parent.assign(2 * n, -1);
        child.assign(2 * n, {-1, -1});
        for (int i = 0; i < n; ++i) {
            value[i] = i;
        }

        sort(edges.begin(), edges.end(), [&](const auto& a, const auto& b) {
            int va = human ? min(a.first, a.second) : max(a.first, a.second);
            int vb = human ? min(b.first, b.second) : max(b.first, b.second);
            return human ? (va > vb) : (va < vb);
        });

        DSU dsu;
        dsu.init(2 * n);

        for (auto [u, v] : edges) {
            int a = dsu.find(u);
            int b = dsu.find(v);
            if (a == b) {
                continue;
            }
            int node = total++;
            value[node] = human ? min(u, v) : max(u, v);
            child[node] = {a, b};
            parent[a] = node;
            parent[b] = node;
            dsu.parent[a] = node;
            dsu.parent[b] = node;
            dsu.parent[node] = node;
        }

        root = dsu.find(0);
        tin.assign(total, 0);
        tout.assign(total, 0);
        timer = 0;
        dfs(root);

        logn = 1;
        while ((1 << logn) <= total) {
            ++logn;
        }
        up.assign(logn, vector<int>(total));
        for (int i = 0; i < total; ++i) {
            up[0][i] = (parent[i] == -1 ? i : parent[i]);
        }
        for (int j = 1; j < logn; ++j) {
            for (int i = 0; i < total; ++i) {
                up[j][i] = up[j - 1][up[j - 1][i]];
            }
        }
    }

    int highest_human(int x, int limit) const {
        if (value[x] < limit) {
            return -1;
        }
        for (int j = logn - 1; j >= 0; --j) {
            int anc = up[j][x];
            if (value[anc] >= limit) {
                x = anc;
            }
        }
        return x;
    }

    int highest_wolf(int x, int limit) const {
        if (value[x] > limit) {
            return -1;
        }
        for (int j = logn - 1; j >= 0; --j) {
            int anc = up[j][x];
            if (value[anc] <= limit) {
                x = anc;
            }
        }
        return x;
    }
};

vector<int> check_validity(int N, vector<int> X, vector<int> Y,
                           vector<int> S, vector<int> E,
                           vector<int> L, vector<int> R) {
    vector<pair<int, int>> edges(X.size());
    for (int i = 0; i < (int)X.size(); ++i) {
        edges[i] = {X[i], Y[i]};
    }

    KruskalTree human_tree, wolf_tree;
    human_tree.build(N, edges, true);
    wolf_tree.build(N, edges, false);

    int size = human_tree.total;
    vector<int> arr(size, -1);
    for (int v = 0; v < N; ++v) {
        arr[human_tree.tin[v]] = wolf_tree.tin[v];
    }

    vector<vector<int>> seg(4 * size);
    function<void(int, int, int)> build = [&](int node, int left, int right) {
        if (left == right) {
            if (arr[left] != -1) {
                seg[node].push_back(arr[left]);
            }
            return;
        }
        int mid = (left + right) / 2;
        build(node * 2, left, mid);
        build(node * 2 + 1, mid + 1, right);
        merge(seg[node * 2].begin(), seg[node * 2].end(),
              seg[node * 2 + 1].begin(), seg[node * 2 + 1].end(),
              back_inserter(seg[node]));
    };
    build(1, 0, size - 1);

    function<bool(int, int, int, int, int, int, int)> query =
        [&](int node, int left, int right, int ql, int qr, int vl, int vr) {
            if (qr < left || right < ql) {
                return false;
            }
            if (ql <= left && right <= qr) {
                auto it = lower_bound(seg[node].begin(), seg[node].end(), vl);
                return it != seg[node].end() && *it <= vr;
            }
            int mid = (left + right) / 2;
            return query(node * 2, left, mid, ql, qr, vl, vr) ||
                   query(node * 2 + 1, mid + 1, right, ql, qr, vl, vr);
        };

    vector<int> ans(S.size(), 0);
    for (int i = 0; i < (int)S.size(); ++i) {
        if (S[i] < L[i] || E[i] > R[i]) {
            continue;
        }
        int hu = human_tree.highest_human(S[i], L[i]);
        int wo = wolf_tree.highest_wolf(E[i], R[i]);
        if (hu == -1 || wo == -1) {
            continue;
        }

        int ql = human_tree.tin[hu];
        int qr = human_tree.tout[hu];
        int vl = wolf_tree.tin[wo];
        int vr = wolf_tree.tout[wo];
        ans[i] = query(1, 0, size - 1, ql, qr, vl, vr) ? 1 : 0;
    }
    return ans;
}

int main() {
    int N, M, Q;
    scanf("%d %d %d", &N, &M, &Q);

    vector<int> X(M), Y(M);
    for (int i = 0; i < M; ++i) {
        scanf("%d %d", &X[i], &Y[i]);
    }

    vector<int> S(Q), E(Q), L(Q), R(Q);
    for (int i = 0; i < Q; ++i) {
        scanf("%d %d %d %d", &S[i], &E[i], &L[i], &R[i]);
    }

    vector<int> ans = check_validity(N, X, Y, S, E, L, R);
    for (int x : ans) {
        printf("%d\n", x);
    }
    return 0;
}
