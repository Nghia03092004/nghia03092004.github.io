bool has_bit(int mask, int bit) {
    return (mask >> bit) & 1;
}

int set_bit(int mask, int bit) {
    return mask | (1 << bit);
}

int clear_bit(int mask, int bit) {
    return mask & ~(1 << bit);
}

vector<int> collect_submasks(int mask) {
    vector<int> subs;
    for (int sub = mask;; sub = (sub - 1) & mask) {
        subs.push_back(sub);
        if (sub == 0) {
            break;
        }
    }
    return subs;
}

vector<long long> subset_sums(const vector<long long>& weight) {
    int n = (int)weight.size();
    vector<long long> sum(1 << n, 0);
    for (int mask = 1; mask < (1 << n); ++mask) {
        int bit = __builtin_ctz(mask);
        sum[mask] = sum[mask ^ (1 << bit)] + weight[bit];
    }
    return sum;
}
