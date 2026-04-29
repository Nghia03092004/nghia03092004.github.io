struct DsuRollback {
    struct Change {
        int child;
        int parent_before;
        int root;
        int size_before;
    };

    vector<int> parent;
    vector<int> size;
    vector<Change> history;
    int components;

    explicit DsuRollback(int n) : parent(n), size(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int v) const {
        while (parent[v] != v) {
            v = parent[v];
        }
        return v;
    }

    int snapshot() const {
        return (int)history.size();
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (size[a] < size[b]) {
            swap(a, b);
        }
        history.push_back({b, parent[b], a, size[a]});
        parent[b] = a;
        size[a] += size[b];
        --components;
        return true;
    }

    void rollback(int snap) {
        while ((int)history.size() > snap) {
            Change ch = history.back();
            history.pop_back();
            parent[ch.child] = ch.parent_before;
            size[ch.root] = ch.size_before;
            ++components;
        }
    }

    bool same(int a, int b) const {
        return find(a) == find(b);
    }
};
