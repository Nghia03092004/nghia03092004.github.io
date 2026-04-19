// IOI 2005 - Mountain
// Segment tree with range-add, clamp-to-zero, and leftmost-threshold query.
// Clamp uses segment-tree-beats style: recurse only on mixed-sign segments.
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000005;

struct Node {
    long long mn, mx;
    long long lazy_add;
    bool lazy_set;
    long long set_val;
};

Node tree[4 * MAXN];
int n;

void build(int v, int l, int r) {
    tree[v] = {0, 0, 0, false, 0};
    if (l == r) return;
    int mid = (l + r) / 2;
    build(2 * v, l, mid);
    build(2 * v + 1, mid + 1, r);
}

void pushDown(int v, int l, int r) {
    if (l == r) return;
    for (int c : {2 * v, 2 * v + 1}) {
        if (tree[v].lazy_set) {
            tree[c].mn = tree[c].mx = tree[v].set_val;
            tree[c].lazy_add = 0;
            tree[c].lazy_set = true;
            tree[c].set_val = tree[v].set_val;
        }
        tree[c].mn += tree[v].lazy_add;
        tree[c].mx += tree[v].lazy_add;
        if (tree[c].lazy_set)
            tree[c].set_val += tree[v].lazy_add;
        else
            tree[c].lazy_add += tree[v].lazy_add;
    }
    tree[v].lazy_add = 0;
    tree[v].lazy_set = false;
}

void pull(int v) {
    tree[v].mn = min(tree[2 * v].mn, tree[2 * v + 1].mn);
    tree[v].mx = max(tree[2 * v].mx, tree[2 * v + 1].mx);
}

void rangeAdd(int v, int l, int r, int ql, int qr, long long val) {
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) {
        tree[v].mn += val;
        tree[v].mx += val;
        if (tree[v].lazy_set) tree[v].set_val += val;
        else tree[v].lazy_add += val;
        return;
    }
    pushDown(v, l, r);
    int mid = (l + r) / 2;
    rangeAdd(2 * v, l, mid, ql, qr, val);
    rangeAdd(2 * v + 1, mid + 1, r, ql, qr, val);
    pull(v);
}

void clampZero(int v, int l, int r) {
    if (tree[v].mn >= 0) return;
    if (tree[v].mx <= 0) {
        tree[v].mn = tree[v].mx = 0;
        tree[v].lazy_add = 0;
        tree[v].lazy_set = true;
        tree[v].set_val = 0;
        return;
    }
    if (l == r) {
        tree[v].mn = tree[v].mx = max(tree[v].mn, 0LL);
        return;
    }
    pushDown(v, l, r);
    int mid = (l + r) / 2;
    clampZero(2 * v, l, mid);
    clampZero(2 * v + 1, mid + 1, r);
    pull(v);
}

int query(int v, int l, int r, int ql, int qr, long long h) {
    if (ql > r || qr < l || tree[v].mx < h) return -1;
    if (l == r) return l;
    pushDown(v, l, r);
    int mid = (l + r) / 2;
    int res = query(2 * v, l, mid, ql, qr, h);
    if (res != -1) return res;
    return query(2 * v + 1, mid + 1, r, ql, qr, h);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    cin >> n >> Q;
    build(1, 1, n);

    while (Q--) {
        char op;
        cin >> op;
        if (op == 'A') {
            int l, r;
            long long val;
            cin >> l >> r >> val;
            rangeAdd(1, 1, n, l, r, val);
        } else if (op == 'M') {
            clampZero(1, 1, n);
        } else { // 'Q'
            long long h;
            cin >> h;
            cout << query(1, 1, n, 1, n, h) << "\n";
        }
    }

    return 0;
}
