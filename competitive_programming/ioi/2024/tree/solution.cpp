#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2024 - Tree
// Rooted tree with N nodes, weights W[i]. Assign coefficients C[i] such that
// for every subtree, L <= sum(C[subtree]) <= R. Minimize sum(|C[i]| * W[i]).
//
// Let S[i] = subtree sum of coefficients for node i. Then C[i] = S[i] - sum(S[child]).
// Choose S[i] in [L, R] for all i to minimize sum(|S[i] - sum(S[children])| * W[i]).
//
// Greedy bottom-up:
//   - Leaves: S[i] = L (minimizes |S[i]| * W[i] since L >= 1).
//   - Internal nodes: S[i] = clamp(sum(S[children]), L, R).
// Per query: O(N).

int N;
vector<int> par, w;
vector<vector<int>> children;
vector<int> order; // bottom-up topological order

void init(vector<int> P, vector<int> W) {
    N = P.size();
    par = P;
    w = W;
    children.resize(N);
    for (int i = 1; i < N; i++)
        children[P[i]].push_back(i);

    // Bottom-up topological order via leaf-first BFS
    order.clear();
    order.reserve(N);
    vector<int> degree(N, 0);
    for (int i = 0; i < N; i++)
        degree[i] = (int)children[i].size();

    queue<int> q;
    for (int i = 0; i < N; i++)
        if (degree[i] == 0)
            q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        if (par[u] >= 0) {
            if (--degree[par[u]] == 0)
                q.push(par[u]);
        }
    }
}

ll query(int L, int R) {
    vector<ll> S(N);
    ll total_cost = 0;

    for (int u : order) {
        if (children[u].empty()) {
            // Leaf: S[u] = L minimizes cost
            S[u] = L;
            total_cost += (ll)L * w[u];
        } else {
            ll child_sum = 0;
            for (int c : children[u])
                child_sum += S[c];

            // Clamp to [L, R]
            S[u] = max((ll)L, min((ll)R, child_sum));
            total_cost += abs(S[u] - child_sum) * w[u];
        }
    }

    return total_cost;
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    vector<int> P(n), W(n);
    P[0] = -1;
    for (int i = 1; i < n; i++) scanf("%d", &P[i]);
    for (int i = 0; i < n; i++) scanf("%d", &W[i]);
    init(P, W);
    while (q--) {
        int L, R;
        scanf("%d %d", &L, &R);
        printf("%lld\n", query(L, R));
    }
    return 0;
}
