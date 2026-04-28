struct SegTree {
    int n;
    vector<long long> tree;

    SegTree() : n(0) {}
    explicit SegTree(const vector<long long>& values) { build(values); }

    void build(const vector<long long>& values) {
        n = (int)values.size();
        tree.assign(2 * n, 0);
        for (int i = 0; i < n; ++i) tree[n + i] = values[i];
        for (int i = n - 1; i > 0; --i) tree[i] = tree[i << 1] + tree[i << 1 | 1];
    }

    void point_set(int pos, long long value) {
        pos += n;
        tree[pos] = value;
        for (pos >>= 1; pos > 0; pos >>= 1) {
            tree[pos] = tree[pos << 1] + tree[pos << 1 | 1];
        }
    }

    void point_add(int pos, long long delta) {
        pos += n;
        tree[pos] += delta;
        for (pos >>= 1; pos > 0; pos >>= 1) {
            tree[pos] = tree[pos << 1] + tree[pos << 1 | 1];
        }
    }

    long long range_sum(int left, int right) const {
        long long left_result = 0, right_result = 0;
        for (left += n, right += n; left <= right; left >>= 1, right >>= 1) {
            if (left & 1) left_result += tree[left++];
            if (!(right & 1)) right_result += tree[right--];
        }
        return left_result + right_result;
    }
};
