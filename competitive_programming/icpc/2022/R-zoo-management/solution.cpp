#include <bits/stdc++.h>
using namespace std;

namespace {

struct Edge {
    int to = 0;
    int id = 0;
};

struct Frame {
    int v = 0;
    int parent_edge = -1;
    int it = 0;
};

vector<int> build_cycle_order(const vector<int>& verts, const vector<vector<Edge>>& graph,
                              const vector<char>& blocked) {
    int start = verts[0];
    int prev = -1;
    int cur = start;
    vector<int> order;
    order.reserve(verts.size());

    do {
        order.push_back(cur);
        int next = -1;
        for (const Edge& e : graph[cur]) {
            if (blocked[e.id]) {
                continue;
            }
            if (e.to == prev) {
                continue;
            }
            next = e.to;
            break;
        }
        prev = cur;
        cur = next;
    } while (cur != start);

    return order;
}

template <class T>
bool is_cyclic_shift(const vector<T>& a, const vector<T>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    if (a.empty()) {
        return true;
    }

    int n = static_cast<int>(b.size());
    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && b[i] != b[j]) {
            j = pi[j - 1];
        }
        if (b[i] == b[j]) {
            ++j;
        }
        pi[i] = j;
    }

    vector<T> text = a;
    text.insert(text.end(), a.begin(), a.end());

    int j = 0;
    for (int i = 0; i < 2 * n - 1; ++i) {
        while (j > 0 && text[i] != b[j]) {
            j = pi[j - 1];
        }
        if (text[i] == b[j]) {
            ++j;
        }
        if (j == n) {
            int pos = i - n + 1;
            if (pos < n) {
                return true;
            }
            j = pi[j - 1];
        }
    }
    return false;
}

bool same_multiset(const vector<int>& verts, const vector<int>& start, const vector<int>& target) {
    vector<int> a;
    vector<int> b;
    a.reserve(verts.size());
    b.reserve(verts.size());
    for (int v : verts) {
        a.push_back(start[v]);
        b.push_back(target[v]);
    }
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    return a == b;
}

bool even_permutation_possible(const vector<int>& verts, const vector<int>& start,
                               const vector<int>& target) {
    unordered_map<int, int> freq;
    freq.reserve(verts.size() * 2);
    for (int v : verts) {
        ++freq[start[v]];
    }
    for (const auto& entry : freq) {
        if (entry.second >= 2) {
            return true;
        }
    }

    unordered_map<int, int> target_pos;
    target_pos.reserve(verts.size() * 2);
    for (int i = 0; i < static_cast<int>(verts.size()); ++i) {
        target_pos[target[verts[i]]] = i;
    }

    vector<int> perm(verts.size());
    for (int i = 0; i < static_cast<int>(verts.size()); ++i) {
        perm[i] = target_pos[start[verts[i]]];
    }

    vector<char> seen(verts.size(), false);
    int cycles = 0;
    for (int i = 0; i < static_cast<int>(verts.size()); ++i) {
        if (seen[i]) {
            continue;
        }
        ++cycles;
        int cur = i;
        while (!seen[cur]) {
            seen[cur] = true;
            cur = perm[cur];
        }
    }

    int parity = (static_cast<int>(verts.size()) - cycles) & 1;
    return parity == 0;
}

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> start(n), target(n);
    for (int i = 0; i < n; ++i) {
        cin >> start[i] >> target[i];
    }

    vector<vector<Edge>> graph(n);
    for (int id = 0; id < m; ++id) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        graph[u].push_back({v, id});
        graph[v].push_back({u, id});
    }

    vector<int> tin(n, 0), low(n, 0), parent(n, -1), parent_edge(n, -1);
    vector<char> is_bridge(m, false);
    int timer = 0;

    for (int root = 0; root < n; ++root) {
        if (tin[root] != 0) {
            continue;
        }

        vector<Frame> stack;
        stack.push_back({root, -1, 0});
        parent[root] = -1;
        parent_edge[root] = -1;

        while (!stack.empty()) {
            Frame& frame = stack.back();
            int v = frame.v;
            if (tin[v] == 0) {
                tin[v] = low[v] = ++timer;
            }

            if (frame.it < static_cast<int>(graph[v].size())) {
                Edge e = graph[v][frame.it++];
                if (e.id == frame.parent_edge) {
                    continue;
                }
                int to = e.to;
                if (tin[to] == 0) {
                    parent[to] = v;
                    parent_edge[to] = e.id;
                    stack.push_back({to, e.id, 0});
                } else {
                    low[v] = min(low[v], tin[to]);
                }
            } else {
                stack.pop_back();
                if (parent[v] != -1) {
                    low[parent[v]] = min(low[parent[v]], low[v]);
                    if (low[v] > tin[parent[v]]) {
                        is_bridge[parent_edge[v]] = true;
                    }
                }
            }
        }
    }

    vector<int> deg_non_bridge(n, 0);
    for (int v = 0; v < n; ++v) {
        for (const Edge& e : graph[v]) {
            if (!is_bridge[e.id]) {
                ++deg_non_bridge[v];
            }
        }
    }

    vector<int> comp_id(n, -1);
    vector<vector<int>> components;
    for (int start_v = 0; start_v < n; ++start_v) {
        if (comp_id[start_v] != -1) {
            continue;
        }
        queue<int> q;
        q.push(start_v);
        comp_id[start_v] = static_cast<int>(components.size());
        components.push_back({});

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            components.back().push_back(v);
            for (const Edge& e : graph[v]) {
                if (is_bridge[e.id]) {
                    continue;
                }
                if (comp_id[e.to] == -1) {
                    comp_id[e.to] = comp_id[start_v];
                    q.push(e.to);
                }
            }
        }
    }

    vector<char> seen(n, false);
    for (const vector<int>& verts : components) {
        if (!same_multiset(verts, start, target)) {
            cout << "impossible\n";
            return;
        }

        long long edge_count = 0;
        bool all_deg_two = true;
        for (int v : verts) {
            edge_count += deg_non_bridge[v];
            if (deg_non_bridge[v] != 2) {
                all_deg_two = false;
            }
        }
        edge_count /= 2;

        if (edge_count == static_cast<long long>(verts.size()) && all_deg_two) {
            vector<int> cycle = build_cycle_order(verts, graph, is_bridge);
            vector<int> a;
            vector<int> b;
            a.reserve(cycle.size());
            b.reserve(cycle.size());
            for (int v : cycle) {
                a.push_back(start[v]);
                b.push_back(target[v]);
            }
            if (!is_cyclic_shift(a, b)) {
                cout << "impossible\n";
                return;
            }
            continue;
        }

        int root = verts[0];
        vector<int> depth(n, -1), delta(n, 0), order;
        vector<int> parent2(n, -1), parent_edge2(n, -1), iter(n, 0);
        bool non_cactus = false;
        bool has_even_cycle = false;

        vector<int> stack;
        stack.push_back(root);
        depth[root] = 0;
        while (!stack.empty()) {
            int v = stack.back();
            if (iter[v] == 0) {
                order.push_back(v);
            }

            if (iter[v] < static_cast<int>(graph[v].size())) {
                Edge e = graph[v][iter[v]++];
                if (is_bridge[e.id] || e.id == parent_edge2[v]) {
                    continue;
                }
                int to = e.to;
                if (depth[to] == -1) {
                    depth[to] = depth[v] + 1;
                    parent2[to] = v;
                    parent_edge2[to] = e.id;
                    stack.push_back(to);
                } else if (depth[to] < depth[v]) {
                    ++delta[v];
                    --delta[to];
                    int cycle_len = depth[v] - depth[to] + 1;
                    if ((cycle_len & 1) == 0) {
                        has_even_cycle = true;
                    }
                }
            } else {
                stack.pop_back();
            }
        }

        for (int i = static_cast<int>(order.size()) - 1; i >= 1; --i) {
            int v = order[i];
            if (delta[v] > 1) {
                non_cactus = true;
            }
            delta[parent2[v]] += delta[v];
        }

        if (non_cactus || has_even_cycle) {
            continue;
        }

        if (!even_permutation_possible(verts, start, target)) {
            cout << "impossible\n";
            return;
        }
    }

    cout << "possible\n";
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
