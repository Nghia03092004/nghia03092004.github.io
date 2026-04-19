// IOI 2004 - Farmer
// Binary search on max distance D. Greedy feasibility check:
// assign each cow (sorted) to leftmost available barn (sorted) within distance D.
// O((N+M) log V) where V is the coordinate range.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<long long> cow(N);
    for (int i = 0; i < N; i++) cin >> cow[i];

    vector<long long> barn(M);
    vector<int> cap(M);
    for (int i = 0; i < M; i++) cin >> barn[i] >> cap[i];

    sort(cow.begin(), cow.end());

    // Sort barns by position
    vector<int> order(M);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return barn[a] < barn[b];
    });
    vector<long long> sb(M);
    vector<int> sc(M);
    for (int i = 0; i < M; i++) {
        sb[i] = barn[order[i]];
        sc[i] = cap[order[i]];
    }

    // Greedy check: can all cows be assigned within distance D?
    auto check = [&](long long D) -> bool {
        vector<int> rem(sc.begin(), sc.end());
        int b = 0;
        for (int i = 0; i < N; i++) {
            // Skip barns too far left or empty
            while (b < M && (sb[b] < cow[i] - D || rem[b] <= 0))
                b++;
            if (b >= M || sb[b] > cow[i] + D) return false;
            rem[b]--;
            // Don't advance b: next cow might use same barn
        }
        return true;
    };

    long long lo = 0, hi = 2000000000LL;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (check(mid))
            hi = mid;
        else
            lo = mid + 1;
    }

    cout << lo << "\n";
    return 0;
}
