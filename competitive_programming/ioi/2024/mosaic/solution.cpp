#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2024 - Mosaic
// N x N grid defined by top row X and left column Y (with X[0] == Y[0]).
// Recurrence: G[i][j] = 1 iff G[i-1][j] == 0 AND G[i][j-1] == 0 (NOR).
// Answer Q rectangle-sum queries using 2D prefix sums.
// Time: O(N^2 + Q).

vector<ll> mosaic(vector<int> X, vector<int> Y,
                  vector<int> T, vector<int> B,
                  vector<int> L, vector<int> R) {
    int N = X.size();
    int Q = T.size();

    // Build the grid
    vector<vector<int>> G(N, vector<int>(N));
    for (int j = 0; j < N; j++) G[0][j] = X[j];
    for (int i = 0; i < N; i++) G[i][0] = Y[i];
    for (int i = 1; i < N; i++)
        for (int j = 1; j < N; j++)
            G[i][j] = (G[i - 1][j] == 0 && G[i][j - 1] == 0) ? 1 : 0;

    // 2D prefix sums (1-indexed)
    vector<vector<ll>> P(N + 1, vector<ll>(N + 1, 0));
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            P[i][j] = G[i - 1][j - 1] + P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1];

    // Answer queries
    vector<ll> ans(Q);
    for (int q = 0; q < Q; q++) {
        int t = T[q], b = B[q], l = L[q], r = R[q];
        ans[q] = P[b + 1][r + 1] - P[t][r + 1] - P[b + 1][l] + P[t][l];
    }

    return ans;
}

int main() {
    int N, Q;
    scanf("%d", &N);
    vector<int> X(N), Y(N);
    for (int i = 0; i < N; i++) scanf("%d", &X[i]);
    for (int i = 0; i < N; i++) scanf("%d", &Y[i]);
    scanf("%d", &Q);
    vector<int> T(Q), B(Q), L(Q), R(Q);
    for (int q = 0; q < Q; q++)
        scanf("%d %d %d %d", &T[q], &B[q], &L[q], &R[q]);
    auto ans = mosaic(X, Y, T, B, L, R);
    for (int q = 0; q < Q; q++)
        printf("%lld\n", ans[q]);
    return 0;
}
