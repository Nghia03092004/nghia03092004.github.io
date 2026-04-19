#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    int n;
    cin >> n;

    const int MAX_TYPE = 1000000;
    vector<int> head(MAX_TYPE + 1, -1);
    vector<int> tail(MAX_TYPE + 1, -1);
    vector<int> cnt_start(MAX_TYPE + 1, 0);
    vector<int> cnt_end(MAX_TYPE + 1, 0);
    vector<int> used_types;

    vector<int> pos(n), delta(n), next_idx(n, -1);
    for (int i = 0; i < n; ++i) {
        string token;
        cin >> token;
        int type = stoi(token.substr(1));
        pos[i] = i + 1;
        delta[i] = (token[0] == 's' ? 1 : -1);
        if (head[type] == -1) {
            used_types.push_back(type);
            head[type] = i;
        } else {
            next_idx[tail[type]] = i;
        }
        tail[type] = i;
        if (delta[i] == 1) {
            ++cnt_start[type];
        } else {
            ++cnt_end[type];
        }
    }

    vector<int> diff(n + 2, 0);
    for (int type : used_types) {
        if (cnt_start[type] != cnt_end[type]) {
            continue;
        }

        int cur = 0;
        int min_value = 0;
        for (int at = head[type]; at != -1; at = next_idx[at]) {
            cur += delta[at];
            min_value = min(min_value, cur);
        }

        cur = 0;
        int left = 1;
        for (int at = head[type]; at != -1; at = next_idx[at]) {
            int right = pos[at];
            if (cur == min_value) {
                ++diff[left];
                --diff[right + 1];
            }
            cur += delta[at];
            left = right + 1;
        }
        if (left <= n && cur == min_value) {
            ++diff[left];
            --diff[n + 1];
        }
    }

    int best_pos = 1;
    int best_score = -1;
    int cur = 0;
    for (int p = 1; p <= n; ++p) {
        cur += diff[p];
        if (cur > best_score) {
            best_score = cur;
            best_pos = p;
        }
    }

    cout << best_pos << ' ' << best_score << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
