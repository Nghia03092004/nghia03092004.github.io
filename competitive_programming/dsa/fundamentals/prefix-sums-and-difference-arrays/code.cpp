struct PrefixSum {
    vector<long long> pref;

    explicit PrefixSum(const vector<long long>& a) : pref(a.size() + 1, 0) {
        for (int i = 0; i < (int)a.size(); ++i) {
            pref[i + 1] = pref[i] + a[i];
        }
    }

    long long query(int l, int r) const {
        return pref[r + 1] - pref[l];
    }
};

struct DifferenceArray {
    vector<long long> diff;

    explicit DifferenceArray(int n) : diff(n + 1, 0) {}

    void add_range(int l, int r, long long delta) {
        diff[l] += delta;
        if (r + 1 < (int)diff.size()) {
            diff[r + 1] -= delta;
        }
    }

    vector<long long> materialize() const {
        vector<long long> a(diff.size() - 1, 0);
        long long running = 0;
        for (int i = 0; i < (int)a.size(); ++i) {
            running += diff[i];
            a[i] = running;
        }
        return a;
    }
};
