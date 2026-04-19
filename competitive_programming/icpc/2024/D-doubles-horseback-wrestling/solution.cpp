#include <bits/stdc++.h>
using namespace std;

namespace {

struct Rider {
    int l;
    int u;
    int id;
};

void solve() {
    int n;
    int s;
    cin >> n >> s;

    vector<Rider> riders(n);
    for (int i = 0; i < n; ++i) {
        cin >> riders[i].l >> riders[i].u;
        riders[i].id = i + 1;
    }

    sort(riders.begin(), riders.end(), [](const Rider& a, const Rider& b) {
        if (a.l != b.l) {
            return a.l < b.l;
        }
        if (a.u != b.u) {
            return a.u < b.u;
        }
        return a.id < b.id;
    });

    vector<char> matched(n, false);
    vector<char> in_pool(n, false);
    vector<pair<int, int>> answer;
    answer.reserve(n / 2);

    multiset<pair<int, int>> pool;  // (u, position in sorted order)
    int ptr = -1;

    for (int pos = n - 1; pos >= 0; --pos) {
        const Rider& cur = riders[pos];
        if (in_pool[pos]) {
            auto it = pool.find({cur.u, pos});
            if (it != pool.end()) {
                pool.erase(it);
            }
            in_pool[pos] = false;
        }
        if (matched[pos]) {
            continue;
        }

        while (ptr + 1 < pos && riders[ptr + 1].l <= s - cur.l) {
            ++ptr;
            if (!matched[ptr] && !in_pool[ptr]) {
                pool.insert({riders[ptr].u, ptr});
                in_pool[ptr] = true;
            }
        }

        auto it = pool.lower_bound({s - cur.u, -1});
        if (it == pool.end()) {
            continue;
        }

        const int other_pos = it->second;
        pool.erase(it);
        in_pool[other_pos] = false;
        matched[pos] = true;
        matched[other_pos] = true;
        answer.push_back({cur.id, riders[other_pos].id});
    }

    cout << answer.size() << '\n';
    for (const auto& p : answer) {
        cout << p.first << ' ' << p.second << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
