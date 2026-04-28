#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

const unsigned long long BASE = 1000000000ULL;

struct Big4 {
    unsigned long long limb[4] = {0, 0, 0, 0};
};

Big4 square_u64(unsigned long long value) {
    unsigned long long low = value % BASE;
    unsigned long long high = value / BASE;
    Big4 result;

    unsigned long long term0 = low * low;
    result.limb[0] = term0 % BASE;
    unsigned long long carry = term0 / BASE;

    unsigned long long term1 = 2ULL * low * high + carry;
    result.limb[1] = term1 % BASE;
    carry = term1 / BASE;

    unsigned long long term2 = high * high + carry;
    result.limb[2] = term2 % BASE;
    result.limb[3] = term2 / BASE;
    return result;
}

void multiply_small(Big4& value, unsigned long long factor) {
    unsigned long long carry = 0;
    for (int i = 0; i < 4; ++i) {
        unsigned long long current = value.limb[i] * factor + carry;
        value.limb[i] = current % BASE;
        carry = current / BASE;
    }
}

int compare_big(const Big4& left, const Big4& right) {
    for (int i = 3; i >= 0; --i) {
        if (left.limb[i] < right.limb[i]) {
            return -1;
        }
        if (left.limb[i] > right.limb[i]) {
            return 1;
        }
    }
    return 0;
}

long long best_denominator(int d, long long bound) {
    long long a0 = static_cast<long long>(sqrt(static_cast<long double>(d)));
    while ((a0 + 1) * (a0 + 1) <= d) {
        ++a0;
    }
    while (a0 * a0 > d) {
        --a0;
    }
    if (a0 * a0 == d) {
        return 0;
    }

    long long m = 0;
    long long den = 1;
    long long a = a0;
    long long q_prev2 = 1;
    long long q_prev1 = 0;

    while (true) {
        long long q_curr = a * q_prev1 + q_prev2;
        if (q_curr > bound) {
            long long t = (bound - q_prev2) / q_prev1;
            if (t <= 0) {
                return q_prev1;
            }

            long long rhs_value = den * (2LL * t * q_prev1 + q_prev2) - m * q_prev1;
            unsigned long long rhs = static_cast<unsigned long long>(rhs_value < 0 ? -rhs_value : rhs_value);
            Big4 left = square_u64(rhs);
            Big4 right = square_u64(static_cast<unsigned long long>(q_prev1));
            multiply_small(right, static_cast<unsigned long long>(d));
            if (compare_big(left, right) > 0) {
                return t * q_prev1 + q_prev2;
            }
            return q_prev1;
        }

        q_prev2 = q_prev1;
        q_prev1 = q_curr;
        m = a * den - m;
        den = (d - m * m) / den;
        a = (a0 + m) / den;
    }
}

int main() {
    const long long bound = 1000000000000LL;
    long long total = 0;

    for (int d = 2; d <= 100000; ++d) {
        total += best_denominator(d, bound);
    }

    assert(total == 57060635927998347LL);
    cout << total << '\n';
    return 0;
}
