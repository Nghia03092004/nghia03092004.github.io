#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

bool find_subset(int n, ll l, ll u, int w[], vector<int> &result) {
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) { return w[a] < w[b]; });

    ll sum = 0;
    int lo = 0, hi = n; // hi = one past last added index in sorted order
    // Add from smallest
    for (lo = 0; lo < n; lo++) {
        sum += w[idx[lo]];
        if (sum >= l) break;
    }

    if (sum < l) return false; // even all elements aren't enough
    if (sum <= u) {
        // Found a valid subset: elements idx[0..lo]
        for (int i = 0; i <= lo; i++) result.push_back(idx[i]);
        return true;
    }

    // sum > u: need to shrink
    // Remove largest elements (from the right of our taken set) and see
    hi = lo; // hi = rightmost taken index in sorted order
    lo = 0;  // lo will be used differently now

    // Two-pointer: taken = [lo, hi] in sorted order
    // sum = sum of w[idx[lo..hi]]
    // We want l <= sum <= u
    while (true) {
        if (sum > u) {
            // Remove largest: subtract w[idx[hi]], hi--
            sum -= w[idx[hi]];
            hi--;
            if (hi < lo) return false; // empty set
        } else if (sum < l) {
            // Need more: this shouldn't happen after initial fill
            // unless we removed too much. Add next element.
            lo--; // This doesn't make sense. Let's rethink.
            return false;
        } else {
            // sum in [l, u]
            for (int i = lo; i <= hi; i++) result.push_back(idx[i]);
            return true;
        }
    }
}

int main() {
    int n;
    ll l, u;
    scanf("%d %lld %lld", &n, &l, &u);
    int w[n];
    for (int i = 0; i < n; i++) scanf("%d", &w[i]);

    vector<int> result;
    if (find_subset(n, l, u, w, result)) {
        printf("%d\n", (int)result.size());
        sort(result.begin(), result.end());
        for (int i = 0; i < (int)result.size(); i++) {
            printf("%d%c", result[i], i+1 < (int)result.size() ? ' ' : '\n');
        }
    } else {
        printf("0\n");
    }
    return 0;
}
