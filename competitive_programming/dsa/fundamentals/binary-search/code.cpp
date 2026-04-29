template <class F>
long long first_true(long long lo, long long hi, F pred) {
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (pred(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

template <class F>
long long last_true(long long lo, long long hi, F pred) {
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;
        if (pred(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

template <class F>
double binary_search_real(double lo, double hi, F pred, int iterations = 80) {
    for (int it = 0; it < iterations; ++it) {
        double mid = (lo + hi) * 0.5;
        if (pred(mid)) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;
}
