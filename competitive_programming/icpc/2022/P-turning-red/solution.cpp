#include <bits/stdc++.h>
using namespace std;

namespace {

int color_value(char c) {
    if (c == 'R') {
        return 0;
    }
    if (c == 'G') {
        return 1;
    }
    return 2;
}

int mod3(int x) {
    x %= 3;
    if (x < 0) {
        x += 3;
    }
    return x;
}

void solve() {
    int l, b;
    cin >> l >> b;

    string colors;
    cin >> colors;

    vector<vector<int>> lights_of_button(b);
    vector<vector<int>> buttons_of_light(l);
    for (int i = 0; i < b; ++i) {
        int k;
        cin >> k;
        lights_of_button[i].resize(k);
        for (int j = 0; j < k; ++j) {
            int light;
            cin >> light;
            --light;
            lights_of_button[i][j] = light;
            buttons_of_light[light].push_back(i);
        }
    }

    vector<int> need(l);
    for (int i = 0; i < l; ++i) {
        need[i] = mod3(-color_value(colors[i]));
        if (buttons_of_light[i].empty() && need[i] != 0) {
            cout << "impossible\n";
            return;
        }
    }

    vector<char> seen(b, false);
    long long answer = 0;
    vector<int> value(b, -1);
    vector<int> component;
    queue<int> q;

    for (int start = 0; start < b; ++start) {
        if (seen[start]) {
            continue;
        }

        component.clear();
        seen[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            component.push_back(u);
            for (int light : lights_of_button[u]) {
                if (buttons_of_light[light].size() != 2) {
                    continue;
                }
                int v = buttons_of_light[light][0] ^ buttons_of_light[light][1] ^ u;
                if (!seen[v]) {
                    seen[v] = true;
                    q.push(v);
                }
            }
        }

        long long best_cost = -1;
        vector<int> best_assignment;
        for (int root_value = 0; root_value < 3; ++root_value) {
            for (int node : component) {
                value[node] = -1;
            }
            queue<int> bfs;
            value[start] = root_value;
            bfs.push(start);

            bool ok = true;
            while (!bfs.empty() && ok) {
                int u = bfs.front();
                bfs.pop();
                for (int light : lights_of_button[u]) {
                    int target = need[light];
                    const auto& owners = buttons_of_light[light];
                    if (owners.size() == 1) {
                        if (value[u] != target) {
                            ok = false;
                            break;
                        }
                        continue;
                    }

                    int v = owners[0] ^ owners[1] ^ u;
                    int required = mod3(target - value[u]);
                    if (value[v] == -1) {
                        value[v] = required;
                        bfs.push(v);
                    } else if (value[v] != required) {
                        ok = false;
                        break;
                    }
                }
            }

            if (!ok) {
                continue;
            }

            long long cost = 0;
            for (int node : component) {
                cost += value[node];
            }
            if (best_cost == -1 || cost < best_cost) {
                best_cost = cost;
                best_assignment.clear();
                best_assignment.reserve(component.size());
                for (int node : component) {
                    best_assignment.push_back(value[node]);
                }
            }
        }

        if (best_cost == -1) {
            cout << "impossible\n";
            return;
        }
        answer += best_cost;
        for (int i = 0; i < static_cast<int>(component.size()); ++i) {
            value[component[i]] = best_assignment[i];
        }
    }

    cout << answer << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
