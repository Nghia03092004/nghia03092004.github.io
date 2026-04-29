struct SqrtDecomposition {
    int n;
    int block_size;
    vector<long long> a;
    vector<long long> block_sum;

    explicit SqrtDecomposition(const vector<long long>& values) : n((int)values.size()), a(values) {
        block_size = max(1, (int)sqrt(n));
        int blocks = (n + block_size - 1) / block_size;
        block_sum.assign(blocks, 0);
        for (int i = 0; i < n; ++i) {
            block_sum[i / block_size] += a[i];
        }
    }

    void point_update(int idx, long long value) {
        int block = idx / block_size;
        block_sum[block] += value - a[idx];
        a[idx] = value;
    }

    long long range_sum(int l, int r) const {
        long long result = 0;
        while (l <= r && l % block_size != 0) {
            result += a[l++];
        }
        while (l + block_size - 1 <= r) {
            result += block_sum[l / block_size];
            l += block_size;
        }
        while (l <= r) {
            result += a[l++];
        }
        return result;
    }
};
