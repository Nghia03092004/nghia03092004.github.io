#include <bits/stdc++.h>
using namespace std;

namespace {

long long c2(long long x) {
    return x * (x - 1) / 2;
}

struct DSU {
    vector<int> parent;
    vector<int> vertices_in_component;
    vector<int> internal_cities;
    vector<unordered_map<int, int>> split_to;
    long long total_shared_component_pairs = 0;
    long long total_double_counted_pairs = 0;

    explicit DSU(int n = 0) {
        init(n);
    }

    void init(int n) {
        parent.resize(n);
        vertices_in_component.assign(n, 1);
        internal_cities.assign(n, 0);
        split_to.assign(n, {});
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            split_to[i].reserve(2);
        }
        total_shared_component_pairs = 0;
        total_double_counted_pairs = 0;
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    int touching_cities(int root) const {
        return vertices_in_component[root] - internal_cities[root];
    }

    void add_initial_city(int a, int b) {
        split_to[a][b] = 1;
        split_to[b][a] = 1;
    }

    void unite(int x, int y) {
        int a = find(x);
        int b = find(y);
        if (a == b) {
            return;
        }

        if (split_to[a].size() + (size_t)vertices_in_component[a] <
            split_to[b].size() + (size_t)vertices_in_component[b]) {
            swap(a, b);
        }

        total_shared_component_pairs -= c2(touching_cities(a));
        total_shared_component_pairs -= c2(touching_cities(b));

        int between = 0;
        auto it_ab = split_to[a].find(b);
        if (it_ab != split_to[a].end()) {
            between = it_ab->second;
            total_double_counted_pairs -= c2(between);
            split_to[a].erase(it_ab);
            split_to[b].erase(a);
        }

        parent[b] = a;
        vertices_in_component[a] += vertices_in_component[b];
        internal_cities[a] += internal_cities[b] + between;

        for (auto it = split_to[b].begin(); it != split_to[b].end(); ++it) {
            int other = it->first;
            int cnt = it->second;
            if (other == a) {
                continue;
            }

            int old = 0;
            auto it_old = split_to[a].find(other);
            if (it_old != split_to[a].end()) {
                old = it_old->second;
            }

            total_double_counted_pairs -= c2(old);
            total_double_counted_pairs -= c2(cnt);
            int merged = old + cnt;
            total_double_counted_pairs += c2(merged);

            split_to[a][other] = merged;
            split_to[other].erase(b);
            split_to[other][a] = merged;
        }
        split_to[b].clear();

        total_shared_component_pairs += c2(touching_cities(a));
    }

    long long answer() const {
        return total_shared_component_pairs - total_double_counted_pairs;
    }
};

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DSU dsu(2 * n);
    for (int city = 0; city < n; ++city) {
        dsu.add_initial_city(city, n + city);
    }

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        --a;
        --b;
        dsu.unite(a, n + b);
        cout << dsu.answer() << '\n';
    }

    return 0;
}
