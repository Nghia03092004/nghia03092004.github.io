// IOI 2003 - Reverse
// Given a sequence of N integers and Q reversal operations, each reversing
// a subarray [l, r] (1-indexed), output the final sequence.
// Uses an implicit-key treap with lazy reversal for O(log N) per operation.
// Complexity: O((N + Q) log N) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

mt19937 rng(42);

struct Node {
    int val, sz, pri;
    bool rev;
    Node *l, *r;
    Node(int v) : val(v), sz(1), pri(rng()), rev(false), l(nullptr), r(nullptr) {}
};

int sz(Node* t) { return t ? t->sz : 0; }

void push(Node* t) {
    if (t && t->rev) {
        swap(t->l, t->r);
        if (t->l) t->l->rev ^= 1;
        if (t->r) t->r->rev ^= 1;
        t->rev = false;
    }
}

void upd(Node* t) {
    if (t) t->sz = 1 + sz(t->l) + sz(t->r);
}

// Split into first k elements (left) and the rest (right)
void split(Node* t, int k, Node*& l, Node*& r) {
    if (!t) { l = r = nullptr; return; }
    push(t);
    if (sz(t->l) + 1 <= k) {
        split(t->r, k - sz(t->l) - 1, t->r, r);
        l = t;
    } else {
        split(t->l, k, l, t->l);
        r = t;
    }
    upd(t);
}

void merge(Node*& t, Node* l, Node* r) {
    push(l);
    push(r);
    if (!l || !r) { t = l ? l : r; return; }
    if (l->pri > r->pri) {
        merge(l->r, l->r, r);
        t = l;
    } else {
        merge(r->l, l, r->l);
        t = r;
    }
    upd(t);
}

// Reverse the subarray [l, r] (1-indexed)
void reverseRange(Node*& root, int l, int r) {
    Node *a, *b, *c;
    split(root, l - 1, a, b);
    split(b, r - l + 1, b, c);
    if (b) b->rev ^= 1;
    merge(b, b, c);
    merge(root, a, b);
}

void inorder(Node* t, vector<int>& res) {
    if (!t) return;
    push(t);
    inorder(t->l, res);
    res.push_back(t->val);
    inorder(t->r, res);
}

// Clean up memory
void destroy(Node* t) {
    if (!t) return;
    destroy(t->l);
    destroy(t->r);
    delete t;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    Node* root = nullptr;
    for (int i = 0; i < N; i++) {
        int x;
        cin >> x;
        Node* node = new Node(x);
        merge(root, root, node);
    }

    int Q;
    cin >> Q;
    while (Q--) {
        int l, r;
        cin >> l >> r;
        if (l < r) { // only reverse if range has more than 1 element
            reverseRange(root, l, r);
        }
    }

    vector<int> result;
    result.reserve(N);
    inorder(root, result);

    for (int i = 0; i < N; i++) {
        cout << result[i] << (i + 1 < N ? ' ' : '\n');
    }

    destroy(root);
    return 0;
}
