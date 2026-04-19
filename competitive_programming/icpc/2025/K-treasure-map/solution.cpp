#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, tx, ty;
    cin >> n >> m >> k >> tx >> ty;

    int total = n + m;
    vector<vector<pair<int, long long>>> adj(total + 1);
    for (int i = 0; i < k; ++i) {
        int x, y;
        long long d;
        cin >> x >> y >> d;
        int row = x;
        int col = n + y;
        adj[row].push_back({col, d});
        adj[col].push_back({row, d});
    }

    vector<int> comp(total + 1, -1);
    vector<long long> val(total + 1, 0);
    vector<char> vis(total + 1, 0);

    const long long INF = (1LL << 60);
    vector<long long> min_row(1, INF), min_col(1, INF);
    vector<char> has_row(1, 0), has_col(1, 0);
    int comps = 0;

    for (int s = 1; s <= total; ++s) {
        if (vis[s]) {
            continue;
        }
        ++comps;
        min_row.push_back(INF);
        min_col.push_back(INF);
        has_row.push_back(0);
        has_col.push_back(0);

        queue<int> q;
        q.push(s);
        vis[s] = 1;
        comp[s] = comps;
        val[s] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (u <= n) {
                has_row[comps] = 1;
                min_row[comps] = min(min_row[comps], val[u]);
            } else {
                has_col[comps] = 1;
                min_col[comps] = min(min_col[comps], val[u]);
            }

            for (const auto& e : adj[u]) {
                int v = e.first;
                long long want = e.second - val[u];
                if (!vis[v]) {
                    vis[v] = 1;
                    comp[v] = comps;
                    val[v] = want;
                    q.push(v);
                } else if (val[v] != want) {
                    cout << "impossible\n";
                    return 0;
                }
            }
        }
    }

    for (int id = 1; id <= comps; ++id) {
        if (has_row[id] && has_col[id] && min_row[id] + min_col[id] < 0) {
            cout << "impossible\n";
            return 0;
        }
    }

    int row_node = tx;
    int col_node = n + ty;
    if (comp[row_node] == comp[col_node]) {
        cout << val[row_node] + val[col_node] << '\n';
    } else {
        long long best = val[row_node] + val[col_node];
        best += -min_row[comp[row_node]];
        best += -min_col[comp[col_node]];
        cout << best << '\n';
    }

    return 0;
}
