struct Fenwick {
    int n;
    vector<long long> bit;

    Fenwick() : n(0) {}
    explicit Fenwick(int n) { init(n); }

    void init(int n_) {
        n = n_;
        bit.assign(n + 1, 0);
    }

    void add(int idx, long long delta) {
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] += delta;
        }
    }

    long long prefix_sum(int idx) const {
        long long result = 0;
        for (; idx > 0; idx -= idx & -idx) {
            result += bit[idx];
        }
        return result;
    }

    long long range_sum(int left, int right) const {
        if (left > right) return 0;
        return prefix_sum(right) - prefix_sum(left - 1);
    }

    // Smallest index idx such that prefix_sum(idx) >= k.
    // Assumes all values are nonnegative and 1 <= k <= total sum.
    int kth(long long k) const {
        int idx = 0;
        int step = 1;
        while ((step << 1) <= n) step <<= 1;
        for (; step > 0; step >>= 1) {
            int next = idx + step;
            if (next <= n && bit[next] < k) {
                idx = next;
                k -= bit[next];
            }
        }
        return idx + 1;
    }
};
