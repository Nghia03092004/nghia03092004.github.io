#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, rank_;
    int components;

    void init(int n) {
        parent.resize(n);
        rank_.resize(n, 0);
        iota(parent.begin(), parent.end(), 0);
        components = n;
    }

    int find(int x) {
        while (parent[x] != x) x = parent[x] = parent[parent[x]];
        return x;
    }

    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
        components--;
        return true;
    }
};

// For IOI grader interface:
// void initialize(int n) - called once
// int hasEdge(int u, int v) - called for each query, return 0 or 1

DSU dsu;
// remaining[i][j] = number of unqueried edges between component i and j
// Since components merge, we use a map
// Actually, we use a different approach: count remaining edges per component pair
// This is complex with merging. Simpler: maintain for each component pair,
// the count of unqueried edges.

// Better approach: use the DSU and a 2D counter.
// But n can be large. Use map<pair<int,int>, int>.

map<pair<int,int>, int> remaining;
int N;

pair<int,int> makeKey(int a, int b) {
    if (a > b) swap(a, b);
    return {a, b};
}

void initialize(int n) {
    N = n;
    dsu.init(n);
    remaining.clear();
    // Initially, between each pair of nodes, there's 1 unqueried edge
    // Between component {i} and component {j}: 1 edge
    // We initialize lazily: remaining edges between comp a and comp b
    // = (size_a * size_b) - (edges already queried between them)
    // Lazy init: if key not in map, compute it.
}

// We need sizes for lazy init
vector<int> comp_size;

void initialize2(int n) {
    N = n;
    dsu.init(n);
    remaining.clear();
    comp_size.assign(n, 1);
}

int hasEdge(int u, int v) {
    int a = dsu.find(u), b = dsu.find(v);
    if (a == b) return 0; // same component, say NO

    auto key = makeKey(a, b);
    if (remaining.find(key) == remaining.end()) {
        // Lazy init: total edges = size_a * size_b
        remaining[key] = comp_size[a] * comp_size[b];
    }
    remaining[key]--;

    if (remaining[key] == 0) {
        // This is the last edge between these components, must say YES
        remaining.erase(key);
        // Merge components, update remaining counts
        int sa = comp_size[a], sb = comp_size[b];

        // For all other components c, merge remaining[a,c] and remaining[b,c]
        // Collect all entries involving a or b
        map<int, int> neighbors;
        vector<pair<int,int>> to_erase;
        for (auto &[k, val] : remaining) {
            if (k.first == a || k.second == a) {
                int other = k.first == a ? k.second : k.first;
                if (other != b) neighbors[other] += val;
                to_erase.push_back(k);
            } else if (k.first == b || k.second == b) {
                int other = k.first == b ? k.second : k.first;
                if (other != a) neighbors[other] += val;
                to_erase.push_back(k);
            }
        }
        for (auto &k : to_erase) remaining.erase(k);

        dsu.unite(a, b);
        int root = dsu.find(a);
        comp_size[root] = sa + sb;

        for (auto &[c, cnt] : neighbors) {
            int rc = dsu.find(c);
            remaining[makeKey(root, rc)] = cnt;
        }

        return 1;
    }
    return 0;
}

int main() {
    // This problem uses grader interaction.
    // Standalone test:
    int n;
    scanf("%d", &n);
    initialize2(n);

    // Simulate all possible queries
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int res = hasEdge(i, j);
            printf("Edge (%d, %d): %d\n", i, j, res);
        }
    }
    return 0;
}
