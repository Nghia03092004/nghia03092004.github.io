#include <bits/stdc++.h>
using namespace std;

namespace {

struct Edge {
    int to = 0;
    int weight = 0;
};

struct State {
    bool covered = false;
    long long value = 0;
};

int village_count;
int fire_engines;
vector<vector<Edge>> tree;
long long centers_used;

State dfs(const int u,
          const int parent,
          const long long edge_to_parent,
          const long long limit) {
    static constexpr long long kInf = (1LL << 60);

    long long closest_center = kInf;
    long long farthest_uncovered = 0;

    for (const Edge& edge : tree[u]) {
        if (edge.to == parent) {
            continue;
        }
        const State child = dfs(edge.to, u, edge.weight, limit);
        if (child.covered) {
            closest_center =
                min(closest_center, child.value + edge.weight);
        } else {
            farthest_uncovered =
                max(farthest_uncovered, child.value + edge.weight);
        }
    }

    if (closest_center + farthest_uncovered <= limit) {
        return {true, closest_center};
    }

    if (parent != -1 && farthest_uncovered + edge_to_parent > limit) {
        ++centers_used;
        return {true, 0};
    }

    return {false, farthest_uncovered};
}

bool can_cover(const long long limit) {
    centers_used = 0;
    const State root = dfs(0, -1, 0, limit);
    if (!root.covered) {
        ++centers_used;
    }
    return centers_used <= fire_engines;
}

void solve() {
    cin >> village_count >> fire_engines;
    tree.assign(village_count, {});

    long long upper_bound = 0;
    for (int i = 1; i < village_count; ++i) {
        int parent;
        int travel_time;
        cin >> parent >> travel_time;
        --parent;
        tree[parent].push_back({i, travel_time});
        tree[i].push_back({parent, travel_time});
        upper_bound += travel_time;
    }

    long long low = -1;
    long long high = upper_bound;
    while (high - low > 1) {
        const long long mid = low + (high - low) / 2;
        if (can_cover(mid)) {
            high = mid;
        } else {
            low = mid;
        }
    }

    cout << high << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
