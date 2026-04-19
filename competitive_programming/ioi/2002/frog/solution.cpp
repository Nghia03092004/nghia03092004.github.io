// IOI 2002 - Frog
// N stones in a circle, frog on stone 1. Frog jumps D positions clockwise,
// removing the departure stone after each jump.
// Uses a Fenwick tree for O(log N) k-th element queries and updates.
// Complexity: O(N log N) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n;
    vector<int> tree;

    BIT(int n) : n(n), tree(n + 1, 0) {}

    void update(int i, int val) {
        for (; i <= n; i += i & (-i))
            tree[i] += val;
    }

    int query(int i) {
        int s = 0;
        for (; i > 0; i -= i & (-i))
            s += tree[i];
        return s;
    }

    // Find the k-th element (1-indexed) using binary lifting on the BIT
    int kth(int k) {
        int pos = 0;
        int bitMask = 1;
        while (bitMask <= n) bitMask <<= 1;
        bitMask >>= 1;

        for (; bitMask > 0; bitMask >>= 1) {
            int next = pos + bitMask;
            if (next <= n && tree[next] < k) {
                k -= tree[next];
                pos = next;
            }
        }
        return pos + 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, D;
    cin >> N >> D;

    BIT bit(N);
    for (int i = 1; i <= N; i++) {
        bit.update(i, 1);
    }

    int remaining = N;
    int currentRank = 1; // rank of current stone among remaining stones

    vector<int> order;
    order.reserve(N);

    while (remaining > 0) {
        int stone = bit.kth(currentRank);
        order.push_back(stone);

        // Remove current stone
        bit.update(stone, -1);
        remaining--;

        if (remaining == 0) break;

        // After removing the stone at currentRank, the stone that was at
        // currentRank+1 shifts down to currentRank. The frog needs to advance
        // D-1 more positions from there (since the departure stone is gone).
        // New rank (0-indexed): (currentRank - 1 + D - 1) % remaining
        currentRank = ((currentRank - 1 + D - 1) % remaining + remaining) % remaining + 1;
    }

    for (int x : order) {
        cout << x << "\n";
    }

    return 0;
}
