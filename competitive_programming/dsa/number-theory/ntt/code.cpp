namespace ntt998244353 {
const int MOD = 998244353;
const int PRIMITIVE_ROOT = 3;

int mod_pow(int base, long long exponent) {
    int result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = (long long)result * base % MOD;
        base = (long long)base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

void ntt(vector<int>& a, bool invert) {
    int n = (int)a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int wlen = mod_pow(PRIMITIVE_ROOT, (MOD - 1) / len);
        if (invert) wlen = mod_pow(wlen, MOD - 2);

        for (int start = 0; start < n; start += len) {
            long long w = 1;
            for (int i = 0; i < len / 2; ++i) {
                int u = a[start + i];
                int v = (int)(w * a[start + i + len / 2] % MOD);
                a[start + i] = u + v < MOD ? u + v : u + v - MOD;
                a[start + i + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                w = w * wlen % MOD;
            }
        }
    }

    if (invert) {
        int inv_n = mod_pow(n, MOD - 2);
        for (int& x : a) x = (long long)x * inv_n % MOD;
    }
}

vector<int> convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};

    int needed = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < needed) n <<= 1;

    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; ++i) {
        a[i] = (long long)a[i] * b[i] % MOD;
    }
    ntt(a, true);
    a.resize(needed);
    return a;
}
}  // namespace ntt998244353
