// IOI 2009 - Rods
// Greedy: sort descending, greedily include all rods and check polygon condition.
// A polygon can be formed iff the longest rod < sum of all others.
// O(N log N) time.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];

    sort(a.rbegin(), a.rend()); // decreasing order

    // Start with all rods. Remove the largest one at a time until condition holds.
    long long totalSum = 0;
    for (int i = 0; i < N; i++) totalSum += a[i];

    // Try removing the largest rods one by one until the polygon condition is met.
    long long sum = totalSum;
    for (int j = 0; j <= N - 3; j++) {
        // With rods a[j], a[j+1], ..., a[N-1]:
        // max is a[j], need sum - a[j] > a[j], i.e., sum > 2 * a[j]
        // and at least 3 rods.
        if ((N - j) >= 3 && sum > 2LL * a[j]) {
            cout << sum << "\n";
            return 0;
        }
        sum -= a[j];
    }

    // No valid polygon possible.
    cout << 0 << "\n";
    return 0;
}
