/*
 * Problem 889: Reversal Sort (Pancake Sorting)
 * BFS on permutation graph to compute pancake distances.
 */
#include <bits/stdc++.h>
using namespace std;

int pancake_number(int n) {
    vector<int> identity(n);
    iota(identity.begin(), identity.end(), 1);
    map<vector<int>, int> dist;
    dist[identity] = 0;
    queue<vector<int>> q;
    q.push(identity);
    int max_d = 0;
    while (!q.empty()) {
        auto perm = q.front(); q.pop();
        int d = dist[perm];
        max_d = max(max_d, d);
        for (int k = 2; k <= n; k++) {
            auto np = perm;
            reverse(np.begin(), np.begin() + k);
            if (dist.find(np) == dist.end()) {
                dist[np] = d + 1;
                q.push(np);
            }
        }
    }
    return max_d;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Pancake Numbers ===" << endl;
    for (int n = 1; n <= 8; n++) {
        int P = pancake_number(n);
        cout << "P(" << n << ") = " << P << endl;
    }

    cout << "\nAnswer: P(8) = " << pancake_number(8) << endl;
    return 0;
}
