#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n;
    vector<int> tree;
    BIT(int n) : n(n), tree(n + 1, 0) {}
    void update(int i, int val) {
        for (i++; i <= n; i += i & (-i))
            tree[i] += val;
    }
    int query(int i) {
        int s = 0;
        for (i++; i > 0; i -= i & (-i))
            s += tree[i];
        return s;
    }
    // number of active elements in [0, i]
};

long long count_swaps(vector<int> S) {
    int n2 = S.size();
    int n = n2 / 2;

    // For each shoe size, store positions of left and right shoes
    map<int, queue<int>> left_pos, right_pos;
    for (int i = 0; i < n2; i++) {
        if (S[i] < 0)
            left_pos[-S[i]].push(i);
        else
            right_pos[S[i]].push(i);
    }

    BIT bit(n2);
    // Initialize: all positions active (value 1)
    for (int i = 0; i < n2; i++)
        bit.update(i, 1);

    vector<bool> used(n2, false);
    long long ans = 0;

    for (int i = 0; i < n2; i++) {
        if (used[i]) continue;

        int shoe = S[i];
        int match_pos;

        if (shoe < 0) {
            // Left shoe, find matching right shoe
            int size = -shoe;
            match_pos = right_pos[size].front();
            right_pos[size].pop();
            left_pos[size].pop(); // this is position i
        } else {
            // Right shoe at left position of pair: need extra swap
            int size = shoe;
            match_pos = left_pos[size].front();
            left_pos[size].pop();
            right_pos[size].pop(); // this is position i
            ans++; // one swap to put left before right
        }

        // Cost: number of active positions between i and match_pos, minus 1
        // (because we bring match_pos next to i)
        int lo = min(i, match_pos);
        int hi = max(i, match_pos);
        int active_between = bit.query(hi) - bit.query(lo);
        ans += active_between;

        used[i] = true;
        used[match_pos] = true;
        bit.update(i, -1);
        bit.update(match_pos, -1);
    }

    return ans;
}

// For local testing
int main() {
    int n;
    scanf("%d", &n);
    vector<int> S(2 * n);
    for (int i = 0; i < 2 * n; i++)
        scanf("%d", &S[i]);
    printf("%lld\n", count_swaps(S));
    return 0;
}
