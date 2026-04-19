// IOI 2009 - Hiring
// Greedy: sort by ratio S[i]/Q[i], sweep with a max-heap of Q values.
// O(N log N) time.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long W;
    cin >> N >> W;

    vector<long long> S(N), Q(N);
    vector<int> idx(N);
    for (int i = 0; i < N; i++) {
        cin >> S[i] >> Q[i];
        idx[i] = i;
    }

    // Sort by ratio S[i]/Q[i] in increasing order (cross-multiply to avoid FP).
    sort(idx.begin(), idx.end(), [&](int a, int b) {
        return S[a] * Q[b] < S[b] * Q[a];
    });

    priority_queue<long long> pq; // max-heap of Q values in current set
    long long sumQ = 0;
    int bestCount = 0;
    double bestCost = 1e18;
    int bestIdx = -1;

    for (int k = 0; k < N; k++) {
        int i = idx[k];
        pq.push(Q[i]);
        sumQ += Q[i];

        // Budget check: S[i] * sumQ <= W * Q[i]  (use __int128 to avoid overflow)
        while (!pq.empty() && (__int128)S[i] * sumQ > (__int128)W * Q[i]) {
            sumQ -= pq.top();
            pq.pop();
        }

        int cnt = (int)pq.size();
        double cost = (double)S[i] / Q[i] * sumQ;
        if (cnt > bestCount || (cnt == bestCount && cost < bestCost)) {
            bestCount = cnt;
            bestCost = cost;
            bestIdx = k;
        }
    }

    cout << bestCount << "\n";

    // Reconstruct: replay up to bestIdx to identify selected workers.
    priority_queue<pair<long long, int>> pq2; // (Q[i], original_index)
    sumQ = 0;
    for (int k = 0; k <= bestIdx; k++) {
        int i = idx[k];
        pq2.push({Q[i], i});
        sumQ += Q[i];

        while (!pq2.empty() &&
               (__int128)S[idx[bestIdx]] * sumQ > (__int128)W * Q[idx[bestIdx]]) {
            sumQ -= pq2.top().first;
            pq2.pop();
        }
    }

    vector<int> selected;
    while (!pq2.empty()) {
        selected.push_back(pq2.top().second + 1); // 1-indexed
        pq2.pop();
    }
    sort(selected.begin(), selected.end());
    for (int x : selected) cout << x << "\n";

    return 0;
}
