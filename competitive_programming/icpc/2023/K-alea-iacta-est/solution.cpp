#include <bits/stdc++.h>
using namespace std;

namespace {

using ld = long double;

struct StateInfo {
    array<unsigned char, 6> digit{};
    unsigned char stars = 0;
    bool fixed = false;
};

int d;
vector<int> pow7;
vector<int> pow6;
vector<StateInfo> info_state;

void enumerate_matches(int id, int idx, int current, const vector<int>& positions,
                       const function<void(int)>& fn) {
    if (idx == static_cast<int>(positions.size())) {
        fn(current);
        return;
    }
    int pos = positions[idx];
    for (int value = 0; value < 6; ++value) {
        enumerate_matches(id, idx + 1, current + (value - 6) * pow7[pos], positions, fn);
    }
}

void solve() {
    int w;
    cin >> d >> w;

    vector<string> dice(d);
    for (int i = 0; i < d; ++i) {
        cin >> dice[i];
    }

    unordered_set<string> goals;
    goals.reserve(w * 2);
    for (int i = 0; i < w; ++i) {
        string word;
        cin >> word;
        sort(word.begin(), word.end());
        goals.insert(word);
    }

    pow7.assign(d + 1, 1);
    pow6.assign(d + 1, 1);
    for (int i = 1; i <= d; ++i) {
        pow7[i] = pow7[i - 1] * 7;
        pow6[i] = pow6[i - 1] * 6;
    }
    int total_states = pow7[d];
    info_state.assign(total_states, {});

    for (int id = 0; id < total_states; ++id) {
        int value = id;
        bool fixed = true;
        int stars = 0;
        for (int i = 0; i < d; ++i) {
            int digit = value % 7;
            value /= 7;
            info_state[id].digit[i] = static_cast<unsigned char>(digit);
            if (digit == 6) {
                fixed = false;
                ++stars;
            }
        }
        info_state[id].fixed = fixed;
        info_state[id].stars = static_cast<unsigned char>(stars);
    }

    const ld INF = 1e100L;
    vector<ld> dist(total_states, INF);
    vector<ld> partial_sum(total_states, 0);
    vector<int> partial_count(total_states, 0);
    vector<char> done(total_states, false);

    using Node = pair<ld, int>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    for (int id = 0; id < total_states; ++id) {
        if (!info_state[id].fixed) {
            continue;
        }
        string current;
        current.reserve(d);
        for (int i = 0; i < d; ++i) {
            current.push_back(dice[i][info_state[id].digit[i]]);
        }
        sort(current.begin(), current.end());
        if (goals.count(current)) {
            dist[id] = 0;
            pq.push({0, id});
        }
    }

    while (!pq.empty()) {
        ld current_dist = pq.top().first;
        int id = pq.top().second;
        pq.pop();
        if (done[id] || fabsl(current_dist - dist[id]) > 1e-18L) {
            continue;
        }
        done[id] = true;

        if (info_state[id].fixed) {
            for (int mask = 1; mask < (1 << d); ++mask) {
                int next_id = id;
                for (int pos = 0; pos < d; ++pos) {
                    if ((mask >> pos) & 1) {
                        next_id += (6 - info_state[id].digit[pos]) * pow7[pos];
                    }
                }
                ++partial_count[next_id];
                partial_sum[next_id] += current_dist;
                int variants = pow6[info_state[next_id].stars];
                ld candidate = (variants + partial_sum[next_id]) / partial_count[next_id];
                if (candidate + 1e-18L < dist[next_id]) {
                    dist[next_id] = candidate;
                    pq.push({candidate, next_id});
                }
            }
        } else {
            vector<int> stars;
            stars.reserve(info_state[id].stars);
            for (int pos = 0; pos < d; ++pos) {
                if (info_state[id].digit[pos] == 6) {
                    stars.push_back(pos);
                }
            }
            enumerate_matches(
                id, 0, id, stars,
                [&](int fixed_id) {
                    if (current_dist + 1e-18L < dist[fixed_id]) {
                        dist[fixed_id] = current_dist;
                        pq.push(make_pair(current_dist, fixed_id));
                    }
                });
        }
    }

    int start_state = 0;
    for (int i = 0; i < d; ++i) {
        start_state += 6 * pow7[i];
    }
    if (dist[start_state] >= INF / 2) {
        cout << "impossible\n";
        return;
    }

    cout << fixed << setprecision(9) << static_cast<double>(dist[start_state]) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
