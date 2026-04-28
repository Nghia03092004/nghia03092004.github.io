struct LazySegTree {
    int n;
    vector<long long> sum, lazy;

    LazySegTree() : n(0) {}
    explicit LazySegTree(const vector<long long>& values) { build(values); }

    void build(const vector<long long>& values) {
        n = (int)values.size();
        sum.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
        build(1, 0, n - 1, values);
    }

    void build(int node, int left, int right, const vector<long long>& values) {
        if (left == right) {
            sum[node] = values[left];
            return;
        }
        int mid = (left + right) >> 1;
        build(node << 1, left, mid, values);
        build(node << 1 | 1, mid + 1, right, values);
        pull(node);
    }

    void apply(int node, int left, int right, long long delta) {
        sum[node] += delta * (right - left + 1);
        lazy[node] += delta;
    }

    void push(int node, int left, int right) {
        if (lazy[node] == 0 || left == right) return;
        int mid = (left + right) >> 1;
        apply(node << 1, left, mid, lazy[node]);
        apply(node << 1 | 1, mid + 1, right, lazy[node]);
        lazy[node] = 0;
    }

    void pull(int node) {
        sum[node] = sum[node << 1] + sum[node << 1 | 1];
    }

    void range_add(int ql, int qr, long long delta) {
        range_add(1, 0, n - 1, ql, qr, delta);
    }

    void range_add(int node, int left, int right, int ql, int qr, long long delta) {
        if (qr < left || right < ql) return;
        if (ql <= left && right <= qr) {
            apply(node, left, right, delta);
            return;
        }
        push(node, left, right);
        int mid = (left + right) >> 1;
        range_add(node << 1, left, mid, ql, qr, delta);
        range_add(node << 1 | 1, mid + 1, right, ql, qr, delta);
        pull(node);
    }

    long long range_sum(int ql, int qr) {
        return range_sum(1, 0, n - 1, ql, qr);
    }

    long long range_sum(int node, int left, int right, int ql, int qr) {
        if (qr < left || right < ql) return 0;
        if (ql <= left && right <= qr) return sum[node];
        push(node, left, right);
        int mid = (left + right) >> 1;
        return range_sum(node << 1, left, mid, ql, qr) +
               range_sum(node << 1 | 1, mid + 1, right, ql, qr);
    }
};
