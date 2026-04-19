#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    int n, c;
    cin >> n >> c;

    vector<long long> t(n);
    for (int i = 0; i < n; ++i) {
        cin >> t[i];
    }
    sort(t.begin(), t.end());

    if (c >= n) {
        cout << t.back() << '\n';
        return;
    }

    const long long INF = (1LL << 62);

    vector<long long> fast_prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        fast_prefix[i + 1] = fast_prefix[i] + t[i];
    }

    // credit_cost[x]: minimum cost of "fast-only" trips that produce x future return credits.
    vector<long long> credit_cost(n, INF);
    credit_cost[0] = 0;
    for (int helpers = 1; helpers < c && helpers < n; ++helpers) {
        long long package_cost = t[helpers] + fast_prefix[helpers + 1];
        for (int have = helpers; have < n; ++have) {
            credit_cost[have] = min(credit_cost[have], credit_cost[have - helpers] + package_cost);
        }
    }

    vector<int> min_credit(n + 1), max_credit(n + 1);
    vector<vector<long long>> dp(n + 1);
    for (int done = 0; done <= n; ++done) {
        min_credit[done] = -(done / c) - (done == n ? 1 : 0);
        max_credit[done] = (n - done + c - 1) / c - 1;
        dp[done].assign(max_credit[done] - min_credit[done] + 1, INF);
    }
    dp[0][-min_credit[0]] = 0;

    for (int done = 0; done < n; ++done) {
        for (int idx = 0; idx < static_cast<int>(dp[done].size()); ++idx) {
            long long current = dp[done][idx];
            if (current >= INF) {
                continue;
            }

            // If producing more credits separately would dominate this state, skip it.
            if (idx > 0 && current - dp[done][0] >= credit_cost[idx]) {
                continue;
            }

            int credit = min_credit[done] + idx;
            int upper = min(n, done + c);
            int extra = -1;
            for (int next = upper; next > done; --next, ++extra) {
                int next_credit = credit + extra;
                if (next_credit > max_credit[next]) {
                    break;
                }
                int helpers = extra + 1;
                long long transition_cost = t[n - 1 - done] + fast_prefix[helpers];
                int next_idx = next_credit - min_credit[next];
                dp[next][next_idx] = min(dp[next][next_idx], current + transition_cost);
            }
        }
    }

    long long answer = INF;
    for (int credit = min_credit[n]; credit <= -1; ++credit) {
        int idx = credit - min_credit[n];
        answer = min(answer, dp[n][idx] + credit_cost[-1 - credit]);
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
