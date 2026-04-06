#include <bits/stdc++.h>
using namespace std;

int main() {
    // P(s,n) = n * ((s-2)*n - (s-4)) / 2
    auto polygonal = [](int s, int n) -> int {
        return n * ((s - 2) * n - (s - 4)) / 2;
    };

    // Generate all 4-digit polygonal numbers for types 3..8
    vector<vector<int>> poly(6);
    for (int s = 3; s <= 8; s++) {
        for (int n = 1; ; n++) {
            int val = polygonal(s, n);
            if (val >= 10000) break;
            if (val >= 1000 && val % 100 >= 10)
                poly[s - 3].push_back(val);
        }
    }

    // Build adjacency: by_prefix[type][prefix] = list of (suffix, value)
    vector<unordered_map<int, vector<pair<int,int>>>> by_prefix(6);
    for (int t = 0; t < 6; t++)
        for (int v : poly[t]) {
            int p = v / 100, s = v % 100;
            by_prefix[t][p].push_back({s, v});
        }

    // DFS to find 6-cycle using all six types
    int answer = 0;
    int chain[6];

    function<void(int, int, int, int)> dfs =
        [&](int depth, int used_mask, int cur_suffix, int start_prefix) {
        if (answer) return;
        if (depth == 6) {
            if (cur_suffix == start_prefix) {
                int sum = 0;
                for (int i = 0; i < 6; i++) sum += chain[i];
                answer = sum;
            }
            return;
        }
        for (int t = 0; t < 6; t++) {
            if (used_mask & (1 << t)) continue;
            auto it = by_prefix[t].find(cur_suffix);
            if (it == by_prefix[t].end()) continue;
            for (auto& [suf, val] : it->second) {
                chain[depth] = val;
                dfs(depth + 1, used_mask | (1 << t), suf, start_prefix);
                if (answer) return;
            }
        }
    };

    for (int t = 0; t < 6 && !answer; t++)
        for (int v : poly[t]) {
            int p = v / 100, s = v % 100;
            chain[0] = v;
            dfs(1, 1 << t, s, p);
            if (answer) break;
        }

    cout << answer << endl;
    return 0;
}
