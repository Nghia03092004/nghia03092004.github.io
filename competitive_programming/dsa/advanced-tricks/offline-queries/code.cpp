struct Fenwick {
    int n;
    vector<int> bit;

    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, int delta) {
        for (++idx; idx <= n; idx += idx & -idx) {
            bit[idx] += delta;
        }
    }

    int sum_prefix(int idx) const {
        int result = 0;
        for (++idx; idx > 0; idx -= idx & -idx) {
            result += bit[idx];
        }
        return result;
    }

    int sum_range(int l, int r) const {
        return sum_prefix(r) - (l ? sum_prefix(l - 1) : 0);
    }
};

struct ThresholdQuery {
    int l;
    int r;
    int x;
    int id;
};

vector<int> count_values_at_most(const vector<int>& a, vector<ThresholdQuery> queries) {
    vector<pair<int, int>> values;
    for (int i = 0; i < (int)a.size(); ++i) {
        values.push_back({a[i], i});
    }
    sort(values.begin(), values.end());
    sort(queries.begin(), queries.end(), [](const ThresholdQuery& lhs, const ThresholdQuery& rhs) {
        return lhs.x < rhs.x;
    });

    Fenwick fw((int)a.size());
    vector<int> answer(queries.size(), 0);
    int ptr = 0;
    for (const ThresholdQuery& q : queries) {
        while (ptr < (int)values.size() && values[ptr].first <= q.x) {
            fw.add(values[ptr].second, 1);
            ++ptr;
        }
        answer[q.id] = fw.sum_range(q.l, q.r);
    }
    return answer;
}
