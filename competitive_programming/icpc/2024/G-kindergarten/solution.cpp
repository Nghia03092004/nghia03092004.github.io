#include <bits/stdc++.h>
using namespace std;

namespace {

struct Solver {
    int n;
    vector<int> jealous;
    vector<int> likes;
    vector<vector<int>> jealous_children;

    vector<int> path;
    vector<int> on_path;
    int loop_start = -1;

    bool dfs(int u, int blocked_parent, int last_jealous_pos) {
        if (on_path[u] != -1) {
            if (on_path[u] >= last_jealous_pos) {
                return false;
            }
            loop_start = on_path[u];
            return true;
        }

        on_path[u] = static_cast<int>(path.size());
        path.push_back(u);

        if (jealous[u] != blocked_parent &&
            dfs(jealous[u], blocked_parent, static_cast<int>(path.size()))) {
            return true;
        }
        if (dfs(likes[u], jealous[u], last_jealous_pos)) {
            return true;
        }

        path.pop_back();
        on_path[u] = -1;
        return false;
    }

    vector<int> build_order() {
        for (int start = 1, steps = 0; steps == 0 || start != 1;
             start = jealous[start], ++steps) {
            loop_start = -1;
            if (dfs(start, jealous[start], 0)) {
                return materialize_order();
            }
        }
        return {};
    }

    vector<int> materialize_order() {
        int loop_prefix = loop_start;
        while (loop_prefix > 0 && jealous[path[loop_prefix - 1]] == path[loop_prefix]) {
            --loop_prefix;
        }

        if (jealous[path.back()] != path[loop_start]) {
            int split = -1;
            for (int i = static_cast<int>(path.size()) - 2; i >= loop_start; --i) {
                if (jealous[path[i]] == path[i + 1]) {
                    split = i + 1;
                    break;
                }
            }
            rotate(path.begin() + loop_start, path.begin() + split, path.end());
        }

        vector<int> used(n + 1, 0);
        vector<int> order;
        vector<int> queue;

        auto emit = [&](int v) {
            if (used[v]) {
                return;
            }
            used[v] = 1;
            order.push_back(v);
            for (int child : jealous_children[v]) {
                queue.push_back(child);
            }
        };

        for (int i = 0; i < static_cast<int>(path.size());) {
            if (loop_prefix <= i && i < loop_start) {
                ++i;
                continue;
            }

            int j = i;
            while (j + 1 < static_cast<int>(path.size()) &&
                   jealous[path[j]] == path[j + 1]) {
                ++j;
            }
            for (int k = j; k >= i; --k) {
                emit(path[k]);
            }
            i = j + 1;
        }

        for (int i = loop_start - 1; i >= loop_prefix; --i) {
            emit(path[i]);
        }
        for (int i = 0; i < static_cast<int>(queue.size()); ++i) {
            emit(queue[i]);
        }

        if (static_cast<int>(order.size()) != n) {
            return {};
        }
        return order;
    }
};

void solve() {
    int n;
    cin >> n;

    Solver solver;
    solver.n = n;
    solver.jealous.assign(n + 1, 0);
    solver.likes.assign(n + 1, 0);
    solver.jealous_children.assign(n + 1, {});
    solver.on_path.assign(n + 1, -1);

    for (int i = 1; i <= n; ++i) {
        int j, l;
        cin >> j >> l;
        solver.jealous[i] = j;
        solver.likes[i] = l;
        solver.jealous_children[j].push_back(i);
    }

    vector<int> order = solver.build_order();
    if (order.empty()) {
        cout << "impossible\n";
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (i) {
            cout << ' ';
        }
        cout << order[i];
    }
    cout << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
