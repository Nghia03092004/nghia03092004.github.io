// IOI 1994 - The Circle
// Bitmask DP: remove adjacent pairs whose sum is divisible by k
// Time: O(2^n * n), Space: O(2^n), n <= 20
#include <bits/stdc++.h>
using namespace std;

int n, k;
int a[21];
int memo[1 << 20];

// Find next active element after pos in circular order
int nextActive(int mask, int pos) {
    for (int step = 1; step < n; step++) {
        int nxt = (pos + step) % n;
        if (mask & (1 << nxt)) return nxt;
    }
    return -1;
}

// Returns maximum number of elements removable from this state
int solve(int mask) {
    if (memo[mask] != -1) return memo[mask];
    if (__builtin_popcount(mask) < 2) return memo[mask] = 0;

    int best = 0;
    // Try each active element and its circular neighbor
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) continue;
        int j = nextActive(mask, i);
        if (j == -1) continue;
        // Only try pair (i, j) where i < j to avoid double counting,
        // but allow the wrap-around pair (last active -> first active)
        if (j < i) continue;
        if ((a[i] + a[j]) % k == 0) {
            int newmask = mask & ~(1 << i) & ~(1 << j);
            best = max(best, 2 + solve(newmask));
        }
    }

    // Also try the wrap-around pair: the largest active index paired
    // with the smallest active index (which nextActive would find,
    // but we skipped it above with j < i check).
    // We need to handle this: find the smallest and largest active bits.
    int lo = -1, hi = -1;
    for (int i = 0; i < n; i++)
        if (mask & (1 << i)) { if (lo == -1) lo = i; hi = i; }
    if (lo != -1 && hi != lo && nextActive(mask, hi) == lo) {
        // (hi, lo) is an adjacent pair in the circle
        if ((a[lo] + a[hi]) % k == 0) {
            int newmask = mask & ~(1 << lo) & ~(1 << hi);
            best = max(best, 2 + solve(newmask));
        }
    }

    return memo[mask] = best;
}

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    if (n <= 1 || k <= 0) {
        printf("0\n");
        return 0;
    }

    memset(memo, -1, sizeof(memo));
    int full = (1 << n) - 1;
    printf("%d\n", solve(full));
    return 0;
}
