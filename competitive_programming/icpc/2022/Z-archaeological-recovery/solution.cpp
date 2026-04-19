#include <bits/stdc++.h>
using namespace std;

namespace {

using u64 = unsigned long long;

int n, k, t_count;
vector<int> pow3;
vector<vector<int>> coord;
vector<int> neg_vec;
vector<vector<int>> add_vec;
vector<vector<int>> dot_mod;

u64 gcd_u64(u64 a, u64 b) {
    while (b != 0) {
        u64 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int encode(const vector<int>& values) {
    int id = 0;
    for (int i = 0; i < k; ++i) {
        id += values[i] * pow3[i];
    }
    return id;
}

int parse_state(const string& s) {
    vector<int> values(k);
    for (int i = 0; i < k; ++i) {
        values[i] = (s[i] == 'A' ? 0 : (s[i] == 'E' ? 1 : 2));
    }
    return encode(values);
}

string lever_string(int id) {
    string s(k, '0');
    for (int i = 0; i < k; ++i) {
        int value = coord[id][i];
        s[i] = (value == 0 ? '0' : (value == 1 ? '+' : '-'));
    }
    return s;
}

void solve() {
    cin >> n >> k >> t_count;

    pow3.assign(k + 1, 1);
    for (int i = 1; i <= k; ++i) {
        pow3[i] = pow3[i - 1] * 3;
    }
    int states = pow3[k];

    coord.assign(states, vector<int>(k, 0));
    neg_vec.assign(states, 0);
    for (int id = 0; id < states; ++id) {
        int x = id;
        vector<int> values(k, 0);
        for (int i = 0; i < k; ++i) {
            values[i] = x % 3;
            x /= 3;
        }
        coord[id] = values;

        vector<int> neg_values(k, 0);
        for (int i = 0; i < k; ++i) {
            neg_values[i] = (3 - values[i]) % 3;
        }
        neg_vec[id] = encode(neg_values);
    }

    dot_mod.assign(states, vector<int>(states, 0));
    add_vec.assign(states, vector<int>(states, 0));
    for (int a = 0; a < states; ++a) {
        for (int b = 0; b < states; ++b) {
            int value = 0;
            int sum_vec = 0;
            for (int i = 0; i < k; ++i) {
                value += coord[a][i] * coord[b][i];
                sum_vec += ((coord[a][i] + coord[b][i]) % 3) * pow3[i];
            }
            dot_mod[a][b] = value % 3;
            add_vec[a][b] = sum_vec;
        }
    }

    vector<u64> target_count(states, 0);
    for (int i = 0; i < t_count; ++i) {
        string s;
        u64 freq;
        cin >> s >> freq;
        target_count[parse_state(s)] = freq;
    }

    vector<int> orth_count(states, 0);
    for (int x = 0; x < states; ++x) {
        u64 cnt[3] = {0, 0, 0};
        for (int s = 0; s < states; ++s) {
            cnt[dot_mod[s][x]] += target_count[s];
        }
        u64 g = gcd_u64(cnt[0], gcd_u64(cnt[1], cnt[2]));
        orth_count[x] = static_cast<int>(__builtin_ctzll(g));
    }

    long long sum_all = 0;
    for (int x = 0; x < states; ++x) {
        sum_all += orth_count[x];
    }
    int zero_count = static_cast<int>((3LL * sum_all / states - n) / 2);

    vector<int> levers;
    levers.reserve(n);
    for (int i = 0; i < zero_count; ++i) {
        levers.push_back(0);
    }

    for (int x = 1; x < states; ++x) {
        if (x > neg_vec[x]) {
            continue;
        }
        long long sum_orth = 0;
        for (int y = 0; y < states; ++y) {
            if (dot_mod[x][y] == 0) {
                sum_orth += orth_count[y];
            }
        }
        int line_count = static_cast<int>((3LL * sum_orth / (states / 3) - n) / 2);
        int multiplicity = line_count - zero_count;
        for (int i = 0; i < multiplicity; ++i) {
            levers.push_back(x);
        }
    }

    vector<u64> dp(states, 0), next_dp(states, 0);
    dp[0] = 1;

    vector<char> reachable(states, false), next_reachable(states, false);
    vector<u64> witness(states, 0), next_witness(states, 0);
    reachable[0] = true;

    for (int i = 0; i < static_cast<int>(levers.size()); ++i) {
        fill(next_dp.begin(), next_dp.end(), 0);
        next_reachable = reachable;
        next_witness = witness;

        int add = levers[i];
        for (int s = 0; s < states; ++s) {
            next_dp[s] += dp[s];
            if (dp[s] != 0) {
                int to = add_vec[s][add];
                next_dp[to] += dp[s];
            }
            if (reachable[s]) {
                int to = add_vec[s][add];
                if (!next_reachable[to]) {
                    next_reachable[to] = true;
                    next_witness[to] = witness[s] | (1ULL << i);
                }
            }
        }

        dp.swap(next_dp);
        reachable.swap(next_reachable);
        witness.swap(next_witness);
    }

    int shift = -1;
    for (int v = 0; v < states; ++v) {
        if (!reachable[v]) {
            continue;
        }
        bool ok = true;
        for (int s = 0; s < states; ++s) {
            int shifted = add_vec[s][v];
            if (dp[shifted] != target_count[s]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            shift = v;
            break;
        }
    }

    vector<int> answer = levers;
    if (shift != -1) {
        u64 mask = witness[shift];
        for (int i = 0; i < static_cast<int>(answer.size()); ++i) {
            if ((mask >> i) & 1ULL) {
                answer[i] = neg_vec[answer[i]];
            }
        }
    }

    for (int id : answer) {
        cout << lever_string(id) << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
