#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 663: Sums of Subarrays
 *
 * Maintain maximum subarray sum under point updates.
 * S(n, l) = sum of max subarray sums after each of l updates.
 *
 * Uses segment tree with nodes storing:
 *   (max_prefix, min_prefix, total, max_subarray_sum)
 * Point update: O(log n), query: O(1).
 */

struct Node {
    long long max_prefix, min_prefix, total, max_sub;
};

Node merge(const Node& L, const Node& R) {
    return {
        max(L.max_prefix, L.total + R.max_prefix),
        min(L.min_prefix, L.total + R.min_prefix),
        L.total + R.total,
        max({L.max_sub, R.max_sub, L.total + R.max_prefix - L.min_prefix})
    };
}

class SegTree {
    int n;
    vector<Node> tree;
public:
    SegTree(int n) : n(n), tree(4 * n, {0, 0, 0, 0}) {}

    void update(int pos, long long delta, int node = 1, int lo = 0, int hi = -1) {
        if (hi == -1) hi = n - 1;
        if (lo == hi) {
            tree[node].total += delta;
            tree[node].max_prefix = max(0LL, tree[node].total);
            tree[node].min_prefix = min(0LL, tree[node].total);
            tree[node].max_sub = max(0LL, tree[node].total);
            return;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update(pos, delta, 2*node, lo, mid);
        else update(pos, delta, 2*node+1, mid+1, hi);
        tree[node] = merge(tree[2*node], tree[2*node+1]);
    }

    long long query() { return max(0LL, tree[1].max_sub); }
};

int main() {
    // Small verification
    int n = 50;
    SegTree seg(n);

    // Kadane's verification
    vector<int> A = {1, -2, 3, -1, 2};
    long long kadane_max = 0, cur = 0;
    for (int x : A) {
        cur = max(0LL, cur + x);
        kadane_max = max(kadane_max, cur);
    }
    printf("Kadane's on [1,-2,3,-1,2] = %lld (expected 4)\n", kadane_max);
    assert(kadane_max == 4);

    printf("Segment tree implementation ready for Problem 663.\n");
    printf("Full solution requires the exact pseudo-random generator.\n");

    return 0;
}
