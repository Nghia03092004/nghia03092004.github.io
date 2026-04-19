#include <bits/stdc++.h>
using namespace std;

namespace {

struct Fenwick {
    int n;
    vector<int> bit;

    explicit Fenwick(int n_) : n(n_), bit(n_ + 1, 0) {}

    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] += val;
        }
    }

    int sum(int idx) const {
        int res = 0;
        for (; idx > 0; idx -= idx & -idx) {
            res += bit[idx];
        }
        return res;
    }

    int range_sum(int l, int r) const {
        return sum(r) - sum(l - 1);
    }
};

void solve() {
    int n, k;
    cin >> n >> k;

    vector<int> to(n + 1), indeg(n + 1, 0);
    vector<vector<int> > rev(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> to[i];
        rev[to[i]].push_back(i);
        ++indeg[to[i]];
    }

    queue<int> q;
    vector<char> in_cycle(n + 1, true);
    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        in_cycle[v] = false;
        int nxt = to[v];
        if (--indeg[nxt] == 0) {
            q.push(nxt);
        }
    }

    vector<vector<int> > cycles;
    vector<char> seen_cycle(n + 1, false);
    vector<int> cycle_id(n + 1, -1), pos_in_cycle(n + 1, -1);
    for (int i = 1; i <= n; ++i) {
        if (!in_cycle[i] || seen_cycle[i]) {
            continue;
        }
        vector<int> cyc;
        int v = i;
        while (!seen_cycle[v]) {
            seen_cycle[v] = true;
            pos_in_cycle[v] = (int)cyc.size();
            cycle_id[v] = (int)cycles.size();
            cyc.push_back(v);
            v = to[v];
        }
        cycles.push_back(cyc);
    }

    vector<int> root_cycle(n + 1, 0);
    vector<int> depth(n + 1, 0);
    vector<int> tin(n + 1), tout(n + 1), euler(1, 0);
    vector<vector<int> > attached(n + 1);
    int timer = 0;

    for (const vector<int>& cyc : cycles) {
        for (int root : cyc) {
            vector<pair<int, int> > st;
            st.push_back(make_pair(root, 0));
            root_cycle[root] = root;
            depth[root] = 0;
            while (!st.empty()) {
                int v = st.back().first;
                int& idx = st.back().second;
                if (idx == 0) {
                    tin[v] = ++timer;
                    euler.push_back(v);
                    attached[root].push_back(v);
                }
                if (idx < (int)rev[v].size()) {
                    int u = rev[v][idx++];
                    if (in_cycle[u]) {
                        continue;
                    }
                    root_cycle[u] = root;
                    depth[u] = depth[v] + 1;
                    st.push_back(make_pair(u, 0));
                } else {
                    tout[v] = timer;
                    st.pop_back();
                }
            }
        }
    }

    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 1);
    sort(nodes.begin(), nodes.end(), [&](int a, int b) {
        return depth[a] < depth[b];
    });

    vector<int> query_nodes(n);
    iota(query_nodes.begin(), query_nodes.end(), 1);
    sort(query_nodes.begin(), query_nodes.end(), [&](int a, int b) {
        return depth[a] + k < depth[b] + k;
    });

    vector<int> answer(n + 1, 0);
    Fenwick fw(n);
    int ptr = 0;
    for (int v : query_nodes) {
        int limit = depth[v] + k;
        while (ptr < n && depth[nodes[ptr]] <= limit) {
            fw.add(tin[nodes[ptr]], 1);
            ++ptr;
        }
        answer[v] = fw.range_sum(tin[v], tout[v]);
    }

    for (const vector<int>& cyc : cycles) {
        int m = (int)cyc.size();
        vector<int> diff(2 * m + 1, 0);
        for (int idx = 0; idx < m; ++idx) {
            int root = cyc[idx];
            for (int v : attached[root]) {
                if (depth[v] > k) {
                    continue;
                }
                int len = min(m - 1, k - depth[v]);
                ++diff[idx];
                --diff[idx + len + 1];
            }
        }
        vector<int> pref(2 * m, 0);
        int cur = 0;
        for (int i = 0; i < 2 * m; ++i) {
            cur += diff[i];
            pref[i] = cur;
        }
        for (int idx = 0; idx < m; ++idx) {
            answer[cyc[idx]] = pref[idx] + pref[idx + m];
        }
    }

    for (int i = 1; i <= n; ++i) {
        cout << answer[i] << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
