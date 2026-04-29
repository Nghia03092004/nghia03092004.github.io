long long mod_pow(long long a, long long e, long long mod) {
    long long result = 1 % mod;
    a %= mod;
    while (e > 0) {
        if (e & 1) {
            result = (__int128)result * a % mod;
        }
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return result;
}

long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long mod_inverse_general(long long a, long long mod) {
    long long x, y;
    long long g = ext_gcd(a, mod, x, y);
    if (g != 1) {
        return -1;
    }
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

long long mod_inverse_prime(long long a, long long mod) {
    return mod_pow(a, mod - 2, mod);
}
