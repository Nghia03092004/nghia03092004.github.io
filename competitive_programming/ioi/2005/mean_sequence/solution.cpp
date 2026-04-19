// IOI 2005 - Mean Sequence
// Given mean sequence b[0..N-1], find original sequence a[0..N] with a[i] >= 0.
// a[i+1] = 2*b[i] - a[i], so a[i] = c[i]*a[0] + d[i] with c alternating +1/-1.
// Find smallest valid a[0] from the constraint interval.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // B[i] = 2*b[i] to keep integer arithmetic
    vector<long long> B(N);
    for (int i = 0; i < N; i++) {
        long long bi;
        cin >> bi;
        B[i] = 2 * bi;
    }

    // a[i] = c[i] * a[0] + d[i]
    vector<int> c(N + 1);
    vector<long long> d(N + 1);
    c[0] = 1;
    d[0] = 0;
    for (int i = 0; i < N; i++) {
        c[i + 1] = -c[i];
        d[i + 1] = B[i] - d[i];
    }

    // Constraints: a[i] >= 0 => c[i]*a[0] + d[i] >= 0
    long long lo = 0, hi = (long long)2e18;
    for (int i = 0; i <= N; i++) {
        if (c[i] == 1)
            lo = max(lo, -d[i]);
        else
            hi = min(hi, d[i]);
    }

    if (lo > hi) {
        cout << "No solution\n";
        return 0;
    }

    long long a0 = lo;
    for (int i = 0; i <= N; i++) {
        if (i > 0) cout << " ";
        cout << (long long)c[i] * a0 + d[i];
    }
    cout << "\n";

    return 0;
}
