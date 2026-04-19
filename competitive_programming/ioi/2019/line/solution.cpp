#include <bits/stdc++.h>
using namespace std;

int minimum_changes(vector<int>& arr) {
    int n = arr.size();
    if (n <= 2) return 0;

    int best = 2; // We can always keep at least 2 elements (or 1)

    // Try all pairs involving positions 0, 1, or 2
    // If the optimal keeps >= 3 elements, at least 2 must be among {0, 1, 2}.
    // Actually, at least one of the first 3 positions must be kept.
    // More precisely: if we keep elements at positions p1 < p2 < p3 < ...,
    // and p1 >= 3, then we only keep n-3 or fewer from positions 3..n-1,
    // so kept <= n-3. If n-3 > best so far, we'd need to try.
    // The standard approach: try pairs from first 3 positions with all others.

    auto try_seq = [&](long long a, long long d) {
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if ((long long)arr[i] == a + (long long)d * i)
                cnt++;
        }
        best = max(best, cnt);
    };

    for (int i = 0; i < min(n, 3); i++) {
        for (int j = i + 1; j < n; j++) {
            long long diff = (long long)arr[j] - arr[i];
            long long gap = j - i;
            if (diff % gap != 0) continue;
            long long d = diff / gap;
            long long a = (long long)arr[i] - d * i;
            try_seq(a, d);
        }
    }

    // Also try keeping just one element with various d values:
    // For n <= small, we could try all pairs. For large n, the above covers it.

    return n - best;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    printf("%d\n", minimum_changes(arr));
    return 0;
}
