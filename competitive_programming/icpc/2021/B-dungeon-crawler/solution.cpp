#include <bits/stdc++.h>
using namespace std;

namespace {

struct Edge {
    int to;
    long long weight;
};

int n;
int q;
long long total_weight = 0;
vector<vector<Edge>> graph;

vector<vector<int>> parent_of;
vector<vector<long long>> dist_from;
vector<vector<long long>> subtree_best;
vector<vector<long long>> outside_best;
vector<vector<long long>> branch_best;
vector<long long> farthest_room;

bool on_path(int start, int mid, int finish) {
    return dist_from[start][mid] + dist_from[mid][finish] == dist_from[start][finish];
}

void preprocess_root(int root) {
    vector<vector<int>> children(n + 1);
    vector<int> order;
    order.reserve(n);

    stack<int> st;
    st.push(root);
    parent_of[root][root] = 0;
    dist_from[root][root] = 0;

    while (!st.empty()) {
        int u = st.top();
        st.pop();
        order.push_back(u);

        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            if (v == parent_of[root][u]) {
                continue;
            }
            parent_of[root][v] = u;
            dist_from[root][v] = dist_from[root][u] + edge.weight;
            children[u].push_back(v);
            st.push(v);
        }
    }

    long long best_distance = 0;
    for (int v = 1; v <= n; ++v) {
        best_distance = max(best_distance, dist_from[root][v]);
    }
    farthest_room[root] = best_distance;

    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        long long best = dist_from[root][u];
        for (int child : children[u]) {
            best = max(best, subtree_best[root][child]);
        }
        subtree_best[root][u] = best;
    }

    for (int u : order) {
        int child_count = static_cast<int>(children[u].size());
        vector<long long> prefix(child_count + 1, dist_from[root][u]);
        vector<long long> suffix(child_count + 1, 0);

        for (int i = 0; i < child_count; ++i) {
            prefix[i + 1] = max(prefix[i], subtree_best[root][children[u][i]]);
        }
        for (int i = child_count - 1; i >= 0; --i) {
            suffix[i] = max(suffix[i + 1], subtree_best[root][children[u][i]]);
        }

        for (int i = 0; i < child_count; ++i) {
            int child = children[u][i];
            branch_best[root][child] = max(prefix[i], suffix[i + 1]);
        }
    }

    vector<int> tin(n + 1, 0);
    vector<int> tout(n + 1, 0);
    vector<int> euler;
    euler.reserve(n);

    stack<pair<int, int>> dfs;
    dfs.push({root, 0});
    while (!dfs.empty()) {
        int u = dfs.top().first;
        int state = dfs.top().second;
        dfs.pop();

        if (state == 0) {
            tin[u] = static_cast<int>(euler.size());
            euler.push_back(u);
            dfs.push({u, 1});
            for (int i = static_cast<int>(children[u].size()) - 1; i >= 0; --i) {
                dfs.push({children[u][i], 0});
            }
        } else {
            tout[u] = static_cast<int>(euler.size()) - 1;
        }
    }

    vector<long long> prefix(n, 0);
    vector<long long> suffix(n, 0);
    for (int i = 0; i < n; ++i) {
        long long value = dist_from[root][euler[i]];
        prefix[i] = (i == 0 ? value : max(prefix[i - 1], value));
    }
    for (int i = n - 1; i >= 0; --i) {
        long long value = dist_from[root][euler[i]];
        suffix[i] = (i + 1 == n ? value : max(suffix[i + 1], value));
    }

    for (int u = 1; u <= n; ++u) {
        long long best = 0;
        if (tin[u] > 0) {
            best = max(best, prefix[tin[u] - 1]);
        }
        if (tout[u] + 1 < n) {
            best = max(best, suffix[tout[u] + 1]);
        }
        outside_best[root][u] = best;
    }
}

long long answer_query(int start, int key, int trap) {
    if (on_path(start, trap, key)) {
        return -1;
    }

    if (on_path(start, key, trap)) {
        return 2LL * total_weight - farthest_room[start];
    }

    int first_special = key;
    while (!on_path(start, parent_of[start][first_special], trap)) {
        first_special = parent_of[start][first_special];
    }

    int branching_point = parent_of[start][first_special];
    long long base_depth = dist_from[start][branching_point];

    long long best_saving = outside_best[start][first_special];
    best_saving = max(
        best_saving,
        subtree_best[start][key] - 2LL * (dist_from[start][key] - base_depth)
    );

    int current = key;
    while (current != first_special) {
        int parent = parent_of[start][current];
        best_saving = max(
            best_saving,
            branch_best[start][current] - 2LL * (dist_from[start][parent] - base_depth)
        );
        current = parent;
    }

    return 2LL * total_weight - best_saving;
}

void solve() {
    cin >> n >> q;
    graph.assign(n + 1, {});
    total_weight = 0;

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
        total_weight += w;
    }

    parent_of.assign(n + 1, vector<int>(n + 1, 0));
    dist_from.assign(n + 1, vector<long long>(n + 1, 0));
    subtree_best.assign(n + 1, vector<long long>(n + 1, 0));
    outside_best.assign(n + 1, vector<long long>(n + 1, 0));
    branch_best.assign(n + 1, vector<long long>(n + 1, 0));
    farthest_room.assign(n + 1, 0);

    for (int root = 1; root <= n; ++root) {
        preprocess_root(root);
    }

    while (q--) {
        int s, k, t;
        cin >> s >> k >> t;

        long long answer = answer_query(s, k, t);
        if (answer < 0) {
            cout << "impossible\n";
        } else {
            cout << answer << '\n';
        }
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
