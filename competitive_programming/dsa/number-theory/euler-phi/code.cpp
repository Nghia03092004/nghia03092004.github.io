long long phi_single(long long n) {
    long long result = n;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p != 0) continue;
        while (n % p == 0) {
            n /= p;
        }
        result -= result / p;
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

vector<int> phi_sieve(int n) {
    vector<int> phi(n + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int p = 2; p <= n; ++p) {
        if (phi[p] != p) continue;
        for (int x = p; x <= n; x += p) {
            phi[x] -= phi[x] / p;
        }
    }
    return phi;
}
