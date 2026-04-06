#include <bits/stdc++.h>
using namespace std;

// Problem 393: Migrating Ants
// Profile DP, row by row, 3 states per column (no vertical swap).
// State encoded in base 3, 3^n possible boundary states.

typedef long long ll;
typedef unordered_map<int, ll> State;

int N;
int pow3[11];

// Decode top state at column j: 0->(0,0), 1->(0,1), 2->(1,0)
pair<int,int> decode(int state, int j) {
    int v = (state / pow3[j]) % 3;
    if (v == 0) return {0, 0};
    if (v == 1) return {0, 1};
    return {1, 0};
}

int encode_bot(int bot_d, int bot_u) {
    if (bot_d == 0 && bot_u == 0) return 0;
    if (bot_d == 0 && bot_u == 1) return 1;
    return 2; // bot_d=1, bot_u=0
}

// Process one row given top boundary state, return map of bottom state -> count
State process_row(int top_state, bool is_last) {
    // Column-by-column DP
    // Key: (r_out, from_right, bot_int) packed as single int
    // r_out and from_right are each 0 or 1, bot_int < 3^N
    // Pack as: bot_int * 4 + r_out * 2 + from_right
    unordered_map<int, ll> dp, new_dp;

    auto pack = [](int r_out, int from_right, int bot_int) {
        return bot_int * 4 + r_out * 2 + from_right;
    };

    // Column 0
    auto [td0, tu0] = decode(top_state, 0);
    int S_in = 1 - td0;
    int S_out = 1 - tu0;
    if (S_in >= 0 && S_in <= 1 && S_out >= 0 && S_out <= 1) {
        // in_choices: (bot_u, from_right) summing to S_in
        // out_choices: (bot_d, r_out) summing to S_out
        vector<pair<int,int>> in_ch, out_ch;
        if (S_in == 0) in_ch = {{0,0}};
        else in_ch = {{0,1},{1,0}};
        if (S_out == 0) out_ch = {{0,0}};
        else out_ch = {{0,1},{1,0}};

        for (auto [bot_u, from_right] : in_ch) {
            for (auto [bot_d, r_out] : out_ch) {
                if (is_last && (bot_d || bot_u)) continue;
                if (bot_d && bot_u) continue;
                if (r_out && from_right) continue;
                int bv = encode_bot(bot_d, bot_u);
                dp[pack(r_out, from_right, bv)] += 1;
            }
        }
    }

    for (int j = 1; j < N; j++) {
        new_dp.clear();
        auto [td_j, tu_j] = decode(top_state, j);
        for (auto& [key, cnt] : dp) {
            int r_in = (key >> 1) & 1;
            int l_demand = key & 1;
            int bot_int = key >> 2;
            if (r_in && l_demand) continue;
            int si = 1 - td_j - r_in;
            int so = 1 - tu_j - l_demand;
            if (si < 0 || si > 1 || so < 0 || so > 1) continue;

            vector<pair<int,int>> in_ch, out_ch;
            if (si == 0) in_ch = {{0,0}};
            else in_ch = {{0,1},{1,0}};
            if (so == 0) out_ch = {{0,0}};
            else out_ch = {{0,1},{1,0}};

            for (auto [bot_u, from_right] : in_ch) {
                for (auto [bot_d, r_out] : out_ch) {
                    if (is_last && (bot_d || bot_u)) continue;
                    if (bot_d && bot_u) continue;
                    if (j == N - 1 && (r_out || from_right)) continue;
                    if (r_out && from_right) continue;
                    int bv = encode_bot(bot_d, bot_u);
                    int new_bot = bot_int + bv * pow3[j];
                    new_dp[pack(r_out, from_right, new_bot)] += cnt;
                }
            }
        }
        swap(dp, new_dp);
    }

    State result;
    for (auto& [key, cnt] : dp) {
        int bot_int = key >> 2;
        result[bot_int] += cnt;
    }
    return result;
}

ll solve(int n) {
    N = n;
    pow3[0] = 1;
    for (int i = 1; i <= n; i++) pow3[i] = pow3[i-1] * 3;

    State dp;
    dp[0] = 1;

    for (int row = 0; row < n; row++) {
        bool is_last = (row == n - 1);
        State new_dp;
        for (auto& [top_int, cnt] : dp) {
            State transitions = process_row(top_int, is_last);
            for (auto& [bot_int, ways] : transitions) {
                new_dp[bot_int] += ways * cnt;
            }
        }
        dp = new_dp;
    }
    return dp.count(0) ? dp[0] : 0;
}

int main() {
    // Verify
    assert(solve(2) == 2);
    assert(solve(4) == 88);

    // Compute f(10)
    ll ans = solve(10);
    cout << ans << endl;
    return 0;
}
