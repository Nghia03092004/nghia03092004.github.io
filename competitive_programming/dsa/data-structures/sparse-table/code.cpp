struct SparseTable {
    int n, lg;
    vector<int> log2_floor;
    vector<vector<int>> st;

    SparseTable() : n(0), lg(0) {}
    explicit SparseTable(const vector<int>& values) { build(values); }

    void build(const vector<int>& values) {
        n = (int)values.size();
        log2_floor.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            log2_floor[i] = log2_floor[i >> 1] + 1;
        }
        lg = log2_floor[n];
        st.assign(lg + 1, vector<int>(n));
        st[0] = values;
        for (int k = 1; k <= lg; ++k) {
            int len = 1 << k;
            for (int i = 0; i + len <= n; ++i) {
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (len >> 1)]);
            }
        }
    }

    int range_min(int left, int right) const {
        int k = log2_floor[right - left + 1];
        return min(st[k][left], st[k][right - (1 << k) + 1]);
    }
};
