#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

long long delivery(int N, int K, int L, int p[]) {
    // p[] is sorted in non-decreasing order
    // leftCost[i] = min cost to deliver boxes 0..i-1 going clockwise
    // rightCost[i] = min cost to deliver boxes n-i..n-1 going counterclockwise

    vector<ll> leftCost(N + 1, 0), rightCost(N + 1, 0);

    for (int i = 1; i <= N; i++) {
        // Deliver box i-1 (0-indexed) going clockwise
        int groupStart = max(0, i - K);
        leftCost[i] = leftCost[groupStart] + 2LL * p[i - 1];
    }

    for (int i = 1; i <= N; i++) {
        int groupStart = max(0, i - K);
        rightCost[i] = rightCost[groupStart] + 2LL * (L - p[N - i]);
    }

    ll ans = LLONG_MAX;
    for (int i = 0; i <= N; i++) {
        // i boxes delivered from left, N-i from right
        ll cost = leftCost[i] + rightCost[N - i];
        ans = min(ans, cost);
    }

    // Also consider combining: some groups go all the way around
    // For each split, the overlapping group costs L instead of 2*left + 2*right
    // This is already handled if we consider mixed groups.
    // Actually, we need to also try: for each (i, j) where i from left, j from right,
    // and some group goes around. The around trip costs L and delivers K boxes.
    // Simpler: for each i from 0..N, and then take min(leftCost[i] + rightCost[N-i])
    // But we also need: leftCost[i] + rightCost[j] + (number of round trips) * L
    // where i + j + roundTrips * K >= N.
    // The above DP already handles non-overlapping. For round trips:
    // We can also compute: for each i, rightCost[j]:
    //   If we combine the last group from left and first group from right into a round trip
    //   cost = leftCost[i-k1] + rightCost[j-k2] + L, where k1+k2 <= K

    // Alternative simpler formulation:
    // The standard approach handles the "go around" case by noting:
    // min over i of leftCost[i] + rightCost[N-i] already works if we also
    // allow the "around" option per group.

    // Let's also try the round-trip version:
    // For each split point i, the last left group and first right group can
    // be merged into a single round trip of cost L (saving 2*p[i-1] + 2*(L-p[i]) - L)
    // if beneficial.
    for (int i = 0; i <= N; i++) {
        // Take i from left, N-i from right, but the boundary groups share a round trip
        int li = i, ri = N - i;
        // Groups from left: ceil(li / K) groups, from right: ceil(ri / K) groups
        // If both > 0, we can merge the last left group and first right group into one
        // round trip if it saves cost.
        // The saved cost per such merge: the last left group costs 2*p[i-1],
        // the first right group costs 2*(L - p[i]), replace both with L.
        if (li > 0 && ri > 0) {
            // Last left group starts at max(0, li-K)
            // First right group starts at max(0, ri-K)
            ll leftPart = leftCost[max(0, li - K)];
            ll rightPart = rightCost[max(0, ri - K)];
            ll cost = leftPart + rightPart + L;
            ans = min(ans, cost);
        }
    }

    return ans;
}

int main() {
    int n, k, l;
    scanf("%d %d %d", &n, &k, &l);
    int p[n];
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);
    sort(p, p + n);
    printf("%lld\n", delivery(n, k, l, p));
    return 0;
}
