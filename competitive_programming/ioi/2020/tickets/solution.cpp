#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2020 - Tickets
// n colors, m tickets each (sorted), k rounds. Each round picks one ticket
// per color. Prize = sum of top n/2 values minus sum of bottom n/2 values.
// Maximize total prize across all rounds.
//
// Greedy: start with all tickets used negatively (smallest k tickets).
// Incrementally convert tickets to positive (largest) to maximize gain.
// Use priority queue on marginal gain of each conversion.

long long find_maximum(int k, vector<vector<int>> x) {
    int n = (int)x.size(), m = (int)x[0].size();
    int half = n / 2;

    // p[i] = number of "positive" (top-half) uses for color i
    vector<int> p(n, 0);

    // Initial total with all k tickets used negatively per color
    ll total_prize = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++)
            total_prize -= x[i][j];

    // Priority queue: (marginal gain, color_index)
    // Gain of incrementing p[i] from t to t+1:
    //   x[i][m-t-1] + x[i][k-1-t]
    priority_queue<pair<ll, int>> pq;
    for (int i = 0; i < n; i++) {
        ll gain = (ll)x[i][m - 1] + x[i][k - 1];
        pq.push({gain, i});
    }

    int total_positive = k * half;
    while (total_positive > 0) {
        auto [gain, i] = pq.top();
        pq.pop();
        total_prize += gain;
        p[i]++;
        total_positive--;

        if (p[i] < k) {
            int t = p[i];
            ll new_gain = (ll)x[i][m - t - 1] + x[i][k - 1 - t];
            pq.push({new_gain, i});
        }
    }

    // Assign tickets to rounds ensuring each round has exactly n/2 positive
    vector<vector<int>> s(n, vector<int>(m, -1));
    vector<vector<int>> pos_rounds(n), neg_rounds(n);

    // Greedy round assignment: process colors by decreasing p[i],
    // assign each to the rounds with fewest positive slots filled
    vector<pair<int, int>> color_order;
    for (int i = 0; i < n; i++)
        color_order.push_back({p[i], i});
    sort(color_order.rbegin(), color_order.rend());

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> round_pq;
    for (int r = 0; r < k; r++) round_pq.push({0, r});

    for (auto [pi, i] : color_order) {
        vector<pair<int, int>> tmp;
        for (int t = 0; t < pi; t++) {
            auto [cnt, r] = round_pq.top();
            round_pq.pop();
            pos_rounds[i].push_back(r);
            tmp.push_back({cnt + 1, r});
        }
        for (auto& pr : tmp) round_pq.push(pr);
    }

    // Negative rounds: all rounds not assigned as positive
    for (int i = 0; i < n; i++) {
        set<int> pos_set(pos_rounds[i].begin(), pos_rounds[i].end());
        for (int r = 0; r < k; r++)
            if (!pos_set.count(r))
                neg_rounds[i].push_back(r);
    }

    // Fill the allocation matrix
    for (int i = 0; i < n; i++) {
        sort(pos_rounds[i].begin(), pos_rounds[i].end());
        for (int t = 0; t < p[i]; t++) {
            int ticket = m - p[i] + t;   // largest tickets
            s[i][ticket] = pos_rounds[i][t];
        }
        sort(neg_rounds[i].begin(), neg_rounds[i].end());
        for (int t = 0; t < k - p[i]; t++) {
            int ticket = t;               // smallest tickets
            s[i][ticket] = neg_rounds[i][t];
        }
    }

    // Output allocation
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", s[i][j]);
        printf("\n");
    }

    return total_prize;
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<vector<int>> x(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &x[i][j]);
    printf("%lld\n", find_maximum(k, x));
    return 0;
}
