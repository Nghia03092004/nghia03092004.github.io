#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>
using namespace std;

/*
 * Problem 101: Optimum Polynomial
 *
 * u(n) = sum_{j=0}^{10} (-1)^j n^j.
 * For k = 1..10, interpolate the first k values and evaluate at k+1.
 * Sum the resulting FITs.
 *
 * Uses exact rational Lagrange interpolation via long long numerator/denominator.
 */

long long u(long long n) {
    long long result = 0, pw = 1;
    for (int j = 0; j <= 10; j++) {
        result += (j % 2 == 0 ? pw : -pw);
        pw *= n;
    }
    return result;
}

long long gcd_abs(long long a, long long b) {
    a = abs(a); b = abs(b);
    while (b) { a %= b; swap(a, b); }
    return a;
}

struct Frac {
    long long num, den;
    Frac(long long n = 0, long long d = 1) : num(n), den(d) { reduce(); }
    void reduce() {
        if (den < 0) { num = -num; den = -den; }
        long long g = gcd_abs(num, den);
        if (g > 0) { num /= g; den /= g; }
    }
    Frac operator*(const Frac& o) const { return Frac(num * o.num, den * o.den); }
    Frac operator+(const Frac& o) const {
        return Frac(num * o.den + o.num * den, den * o.den);
    }
};

int main() {
    vector<long long> vals(12);
    for (int n = 1; n <= 11; n++) vals[n] = u(n);

    long long total = 0;
    for (int k = 1; k <= 10; k++) {
        long long target = k + 1;
        Frac result(0, 1);
        for (int i = 1; i <= k; i++) {
            Frac term(vals[i], 1);
            for (int j = 1; j <= k; j++) {
                if (j == i) continue;
                term = term * Frac(target - j, i - j);
            }
            result = result + term;
        }
        assert(result.den == 1);
        total += result.num;
    }

    cout << total << endl;
    assert(total == 37076114526LL);
    return 0;
}
