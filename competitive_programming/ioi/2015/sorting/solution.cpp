#include <bits/stdc++.h>
using namespace std;

int findSwapPairs(int n, int S[], int m,
                  int P[], int Q[],  // grader's swaps (size m)
                  int X[], int Y[])  // our swaps (output)
{
    // Binary search on number of rounds
    int lo = 0, hi = m;

    auto check = [&](int rounds) -> int {
        // Simulate S after 'rounds' grader swaps
        vector<int> T(S, S + n);
        for (int i = 0; i < rounds; i++) {
            swap(T[P[i]], T[Q[i]]);
        }
        // Count cycles
        vector<bool> visited(n, false);
        int cycles = 0;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                cycles++;
                int j = i;
                while (!visited[j]) {
                    visited[j] = true;
                    j = T[j];
                }
            }
        }
        return n - cycles <= rounds;
    };

    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (check(mid)) hi = mid;
        else lo = mid + 1;
    }

    int ans = lo;

    // Construct the swap sequence
    // Compute S after 'ans' grader swaps
    vector<int> T(S, S + n);
    for (int i = 0; i < ans; i++) {
        swap(T[P[i]], T[Q[i]]);
    }

    // Find swaps to sort T: process cycles
    // planned[k] = (element_a, element_b) to swap in step k (in terms of elements, not positions)
    vector<pair<int,int>> planned;
    vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
        if (!visited[i] && T[i] != i) {
            // Trace cycle starting at i
            vector<int> cycle;
            int j = i;
            while (!visited[j]) {
                visited[j] = true;
                cycle.push_back(j);
                j = T[j];
            }
            // Sort this cycle: swap cycle[0] with cycle[k] for k = len-1 down to 1
            for (int k = (int)cycle.size() - 1; k >= 1; k--) {
                planned.push_back({cycle[0], cycle[k]});
            }
        }
    }

    // Pad with dummy swaps
    while ((int)planned.size() < ans) {
        planned.push_back({0, 0});
    }

    // Now simulate forward and adjust
    // pos[v] = current position of element v
    // elem[p] = element at position p
    vector<int> pos(n), elem(n);
    for (int i = 0; i < n; i++) {
        elem[i] = S[i];
        pos[S[i]] = i;
    }

    for (int i = 0; i < ans; i++) {
        // Grader swap: swap positions P[i] and Q[i]
        int a = elem[P[i]], b = elem[Q[i]];
        swap(elem[P[i]], elem[Q[i]]);
        pos[a] = Q[i];
        pos[b] = P[i];

        // Our swap: we want to swap elements planned[i].first and planned[i].second
        int u = planned[i].first, v = planned[i].second;
        X[i] = pos[u];
        Y[i] = pos[v];
        // Apply our swap
        swap(elem[X[i]], elem[Y[i]]);
        pos[u] = Y[i];
        pos[v] = X[i];
    }

    return ans;
}

int main() {
    int n;
    scanf("%d", &n);
    int S[n];
    for (int i = 0; i < n; i++) scanf("%d", &S[i]);
    int m;
    scanf("%d", &m);
    int P[m], Q[m];
    for (int i = 0; i < m; i++) scanf("%d %d", &P[i], &Q[i]);
    int X[m], Y[m];
    int ans = findSwapPairs(n, S, m, P, Q, X, Y);
    printf("%d\n", ans);
    for (int i = 0; i < ans; i++) printf("%d %d\n", X[i], Y[i]);
    return 0;
}
