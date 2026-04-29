template <class Cost>
void compute_dp_row(int left, int right, int opt_left, int opt_right,
                    const vector<long long>& prev, vector<long long>& cur, Cost cost) {
    if (left > right) {
        return;
    }
    int mid = (left + right) / 2;
    pair<long long, int> best = {(long long)4e18, -1};

    int upper = min(mid, opt_right);
    for (int k = opt_left; k <= upper; ++k) {
        long long candidate = (k == 0 ? 0 : prev[k - 1]) + cost(k, mid);
        if (candidate < best.first) {
            best = {candidate, k};
        }
    }

    cur[mid] = best.first;
    compute_dp_row(left, mid - 1, opt_left, best.second, prev, cur, cost);
    compute_dp_row(mid + 1, right, best.second, opt_right, prev, cur, cost);
}
