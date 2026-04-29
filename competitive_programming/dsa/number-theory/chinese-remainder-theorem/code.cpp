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

bool combine_crt(long long r1, long long m1, long long r2, long long m2, long long& r, long long& m) {
    long long x, y;
    long long g = ext_gcd(m1, m2, x, y);
    long long diff = r2 - r1;
    if (diff % g != 0) {
        return false;
    }

    long long lcm = m1 / g * m2;
    long long step = m2 / g;
    long long t = (__int128)(diff / g) * x % step;
    if (t < 0) t += step;
    r = (r1 + (__int128)m1 * t) % lcm;
    if (r < 0) r += lcm;
    m = lcm;
    return true;
}

pair<long long, long long> chinese_remainder(const vector<long long>& rem, const vector<long long>& mod) {
    long long r = 0;
    long long m = 1;
    for (int i = 0; i < (int)rem.size(); ++i) {
        if (!combine_crt(r, m, rem[i], mod[i], r, m)) {
            return {-1, -1};
        }
    }
    return {r, m};
}
