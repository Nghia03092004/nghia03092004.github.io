struct RollingHash {
    static constexpr long long MOD = 1'000'000'007;
    static constexpr long long BASE = 911382323;

    vector<long long> power;
    vector<long long> pref;

    explicit RollingHash(const string& s) : power(s.size() + 1, 1), pref(s.size() + 1, 0) {
        for (int i = 0; i < (int)s.size(); ++i) {
            power[i + 1] = (__int128)power[i] * BASE % MOD;
            pref[i + 1] = (( __int128)pref[i] * BASE + s[i]) % MOD;
        }
    }

    long long get_hash(int l, int r) const {
        long long value = (pref[r + 1] - (__int128)pref[l] * power[r - l + 1]) % MOD;
        if (value < 0) value += MOD;
        return value;
    }
};
