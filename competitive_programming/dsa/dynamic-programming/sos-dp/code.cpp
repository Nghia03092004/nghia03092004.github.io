vector<long long> sos_subsets(vector<long long> f, int bits) {
    for (int bit = 0; bit < bits; ++bit) {
        for (int mask = 0; mask < (1 << bits); ++mask) {
            if ((mask >> bit) & 1) {
                f[mask] += f[mask ^ (1 << bit)];
            }
        }
    }
    return f;
}

vector<long long> sos_supersets(vector<long long> f, int bits) {
    for (int bit = 0; bit < bits; ++bit) {
        for (int mask = 0; mask < (1 << bits); ++mask) {
            if (((mask >> bit) & 1) == 0) {
                f[mask] += f[mask | (1 << bit)];
            }
        }
    }
    return f;
}
