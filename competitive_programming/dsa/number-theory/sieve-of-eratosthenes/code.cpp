struct PrimeSieve {
    int limit;
    vector<int> primes;
    vector<int> spf;

    explicit PrimeSieve(int limit) : limit(limit), spf(limit + 1, 0) {
        for (int i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || 1LL * i * p > limit) {
                    break;
                }
                spf[i * p] = p;
            }
        }
    }

    bool is_prime(int x) const {
        return x >= 2 && spf[x] == x;
    }

    vector<pair<int, int>> factorize(int x) const {
        vector<pair<int, int>> factors;
        while (x > 1) {
            int p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            factors.push_back({p, cnt});
        }
        return factors;
    }
};
