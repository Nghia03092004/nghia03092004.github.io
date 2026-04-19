// IOI 2006 - Beans (Mexicaneno)
// Max non-adjacent subset sum on a circular arrangement.
// Split into two linear cases: exclude first, or include first (exclude neighbors).
// O(N).
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<long long> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];

    if (N == 0) { cout << 0 << "\n"; return 0; }
    if (N == 1) { cout << a[0] << "\n"; return 0; }

    // Solve max non-adjacent subset sum on a linear array
    auto solveLinear = [](const long long* arr, int n) -> long long {
        if (n == 0) return 0;
        if (n == 1) return max(0LL, arr[0]);
        long long prev_no = 0, prev_yes = arr[0];
        for (int i = 1; i < n; i++) {
            long long new_no = max(prev_no, prev_yes);
            long long new_yes = prev_no + arr[i];
            prev_no = new_no;
            prev_yes = new_yes;
        }
        return max(prev_no, prev_yes);
    };

    // Case 1: exclude a[0], solve on a[1..N-1]
    long long ans1 = solveLinear(a.data() + 1, N - 1);

    // Case 2: include a[0], exclude a[1] and a[N-1], solve on a[2..N-2]
    long long ans2 = a[0];
    if (N >= 3)
        ans2 += solveLinear(a.data() + 2, N - 3);

    cout << max(ans1, ans2) << "\n";
    return 0;
}
