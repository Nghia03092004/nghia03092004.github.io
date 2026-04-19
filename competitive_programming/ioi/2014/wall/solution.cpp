#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2000005;

int lo[4 * MAXN], hi[4 * MAXN];

void init(int node, int l, int r) {
    lo[node] = 0;
    hi[node] = 1 << 30;
    if (l == r) return;
    int mid = (l + r) / 2;
    init(2 * node, l, mid);
    init(2 * node + 1, mid + 1, r);
}

void pushDown(int node) {
    for (int child : {2 * node, 2 * node + 1}) {
        // Compose: clamp child's [lo,hi] by parent's [lo,hi]
        lo[child] = min(max(lo[child], lo[node]), hi[node]);
        hi[child] = min(max(hi[child], lo[node]), hi[node]);
    }
    lo[node] = 0;
    hi[node] = 1 << 30;
}

// op=1: add (clamp_low), op=2: remove (clamp_high)
void update(int node, int l, int r, int ql, int qr, int op, int h) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        if (op == 1) {
            // clamp_low(h): raise lo and hi if needed
            lo[node] = max(lo[node], h);
            hi[node] = max(hi[node], h);
        } else {
            // clamp_high(h): lower hi and lo if needed
            hi[node] = min(hi[node], h);
            lo[node] = min(lo[node], h);
        }
        return;
    }
    pushDown(node);
    int mid = (l + r) / 2;
    update(2 * node, l, mid, ql, qr, op, h);
    update(2 * node + 1, mid + 1, r, ql, qr, op, h);
}

void query(int node, int l, int r, int ans[]) {
    if (l == r) {
        // The final value is 0 clamped by [lo, hi]
        ans[l] = min(max(0, lo[node]), hi[node]);
        return;
    }
    pushDown(node);
    int mid = (l + r) / 2;
    query(2 * node, l, mid, ans);
    query(2 * node + 1, mid + 1, r, ans);
}

void buildWall(int n, int k, int op[], int left[], int right[],
               int height[], int finalHeight[]) {
    init(1, 0, n - 1);
    for (int i = 0; i < k; i++) {
        update(1, 0, n - 1, left[i], right[i], op[i], height[i]);
    }
    query(1, 0, n - 1, finalHeight);
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    vector<int> op(k), left(k), right(k), height(k);
    for (int i = 0; i < k; i++) {
        scanf("%d %d %d %d", &op[i], &left[i], &right[i], &height[i]);
    }
    vector<int> ans(n);
    buildWall(n, k, op.data(), left.data(), right.data(),
              height.data(), ans.data());
    for (int i = 0; i < n; i++) {
        printf("%d\n", ans[i]);
    }
    return 0;
}
