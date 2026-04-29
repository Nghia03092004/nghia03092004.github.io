long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a >= 0 ? a : -a;
}

long long lcm_ll(long long a, long long b) {
    return a / gcd_ll(a, b) * b;
}

long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return a >= 0 ? a : -a;
    }
    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

bool solve_linear_diophantine(long long a, long long b, long long c, long long& x, long long& y, long long& g) {
    g = ext_gcd(a, b, x, y);
    if (c % g != 0) {
        return false;
    }
    x *= c / g;
    y *= c / g;
    return true;
}
