#include <iostream>
#include <cmath>
using namespace std;

/*
 * Problem 104: Pandigital Fibonacci Ends
 *
 * Maintain F_k mod 10^9 for trailing digits. Use the logarithmic
 * formula for leading digits. Return the first k where both ends
 * are 1-9 pandigital.
 */

bool is_pandigital(long long n) {
    if (n < 123456789 || n > 987654321) return false;
    int seen[10] = {};
    for (int i = 0; i < 9; i++) {
        int d = n % 10;
        n /= 10;
        if (d == 0 || ++seen[d] > 1) return false;
    }
    return true;
}

int main() {
    const long long MOD = 1000000000LL;
    const double LOG10_PHI = log10((1.0 + sqrt(5.0)) / 2.0);
    const double LOG10_SQRT5 = 0.5 * log10(5.0);

    long long a = 1, b = 1;
    for (int k = 3; ; k++) {
        long long c = (a + b) % MOD;
        a = b;
        b = c;
        if (!is_pandigital(c)) continue;
        double L = (double)k * LOG10_PHI - LOG10_SQRT5;
        double frac = L - floor(L);
        long long first9 = (long long)pow(10.0, frac + 8.0);
        if (is_pandigital(first9)) {
            cout << k << endl;
            return 0;
        }
    }
}
