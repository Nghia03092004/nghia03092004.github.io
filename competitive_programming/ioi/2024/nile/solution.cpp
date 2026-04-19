#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2024 - Nile
// N artifacts with weight W[i], solo cost A[i], paired cost B[i] (B[i] < A[i]).
// Two artifacts can pair iff |W[i] - W[j]| <= D.
// For Q queries with different D, find minimum total transport cost.
//
// Sort by weight. DP on sorted order: either ship artifact alone (cost A[i])
// or pair with previous artifact if weight difference <= D (cost B[i] + B[i-1]).
// Pairing adjacent in sorted order is optimal.
// Time: O(N log N + NQ).

vector<ll> calculate_costs(vector<int> W, vector<int> A,
                           vector<int> B, vector<int> E) {
    int N = W.size();
    int Q = E.size();

    // Sort artifacts by weight
    vector<int> idx(N);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) {
        return W[a] < W[b];
    });

    vector<int> sw(N), sa(N), sb(N);
    for (int i = 0; i < N; i++) {
        sw[i] = W[idx[i]];
        sa[i] = A[idx[i]];
        sb[i] = B[idx[i]];
    }

    vector<ll> ans(Q);
    for (int q = 0; q < Q; q++) {
        ll D = E[q];

        // dp[i] = min cost for first i sorted artifacts
        vector<ll> dp(N + 1, 0);
        for (int i = 1; i <= N; i++) {
            // Option 1: ship artifact i-1 alone
            dp[i] = dp[i - 1] + sa[i - 1];

            // Option 2: pair with previous artifact
            if (i >= 2 && (ll)(sw[i - 1] - sw[i - 2]) <= D)
                dp[i] = min(dp[i], dp[i - 2] + (ll)sb[i - 1] + sb[i - 2]);
        }

        ans[q] = dp[N];
    }

    return ans;
}

int main() {
    int N, Q;
    scanf("%d", &N);
    vector<int> W(N), A(N), B(N);
    for (int i = 0; i < N; i++)
        scanf("%d %d %d", &W[i], &A[i], &B[i]);
    scanf("%d", &Q);
    vector<int> E(Q);
    for (int i = 0; i < Q; i++) scanf("%d", &E[i]);
    auto ans = calculate_costs(W, A, B, E);
    for (int q = 0; q < Q; q++)
        printf("%lld\n", ans[q]);
    return 0;
}
