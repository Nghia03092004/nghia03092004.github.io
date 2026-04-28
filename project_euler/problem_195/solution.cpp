#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>

using namespace std;

long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

long long count_triangles(long long limit) {
    const long double sqrt3 = sqrt(static_cast<long double>(3.0));
    const long double limit_plain = 2.0L * limit / sqrt3;
    const long double limit_div3 = 6.0L * limit / sqrt3;
    const long long q_max = static_cast<long long>(sqrt(limit_div3)) + 2;
    long long total = 0;

    for (long long q = 1; q <= q_max; ++q) {
        long long p_max = static_cast<long long>(limit_div3 / q);
        for (long long p = q + 1; p <= p_max; ++p) {
            if (gcd_ll(p, q) != 1) {
                continue;
            }
            long long product = p * q;
            if ((p - q) % 3 == 0) {
                total += static_cast<long long>(limit_div3 / product);
            } else {
                total += static_cast<long long>(limit_plain / product);
            }
        }
    }

    return total;
}

int main() {
    assert(count_triangles(100) == 1234);
    assert(count_triangles(1000) == 22767);
    assert(count_triangles(10000) == 359912);

    long long answer = count_triangles(1053779);
    assert(answer == 75085391LL);
    cout << answer << '\n';
    return 0;
}
