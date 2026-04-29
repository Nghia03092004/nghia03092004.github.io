struct Edge {
    int u;
    int v;
    long long w;
};

struct Dsu {
    vector<int> parent;
    vector<int> size;

    explicit Dsu(int n) : parent(n), size(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int v) {
        if (parent[v] == v) return v;
        return parent[v] = find(parent[v]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

pair<long long, vector<Edge>> kruskal_mst(int n, vector<Edge> edges) {
    sort(edges.begin(), edges.end(), [](const Edge& lhs, const Edge& rhs) {
        return lhs.w < rhs.w;
    });

    Dsu dsu(n);
    long long total = 0;
    vector<Edge> chosen;
    for (const Edge& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            chosen.push_back(e);
        }
    }
    if ((int)chosen.size() != n - 1) {
        return {-1, {}};
    }
    return {total, chosen};
}
