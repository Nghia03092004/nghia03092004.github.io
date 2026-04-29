struct DSU {
    vector<int> parent;  // root stores -size, non-root stores parent
    int components = 0;

    DSU() = default;
    explicit DSU(int n) { init(n); }

    void init(int n) {
        parent.assign(n, -1);
        components = n;
    }

    int find(int x) {
        if (parent[x] < 0) return x;
        return parent[x] = find(parent[x]);
    }

    int leader(int x) {
        return find(x);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (-parent[a] < -parent[b]) swap(a, b);
        parent[a] += parent[b];
        parent[b] = a;
        --components;
        return true;
    }

    bool same(int a, int b) {
        return find(a) == find(b);
    }

    int size(int x) {
        return -parent[find(x)];
    }

    int component_count() const {
        return components;
    }
};
