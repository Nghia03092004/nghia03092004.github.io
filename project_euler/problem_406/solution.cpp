#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 406: Guessing Game
 *
 * C(n, a, b) = minimum worst-case cost to find a number in {1..n}
 * with cost a for "too low" and cost b for "too high".
 *
 * Key insight: f(W) = max range searchable with worst-case cost <= W.
 * f(W) = 0 if W < 0
 * f(W) = 1 if 0 <= W < min(a,b) (can only hold 1 number)
 * f(W) = f(W-a) + f(W-b) + 1 otherwise
 *
 * C(n, a, b) = min W such that f(W) >= n.
 * Binary search on W.
 *
 * For computation: states (i,j) meaning i times cost-a deducted,
 * j times cost-b deducted. Budget = W - i*a - j*b.
 */

long double solve(long long n, long double a, long double b) {
    long double hi = log2l((long double)n) * (a + b) * 2.0L;
    long double lo = 0;

    for (int iter = 0; iter < 300; iter++) {
        long double mid = (lo + hi) / 2.0L;

        map<pair<int,int>, long long> memo;

        function<long long(int, int)> f = [&](int i, int j) -> long long {
            long double budget = mid - i * a - j * b;
            if (budget < -1e-12L) return 0LL;

            auto key = make_pair(i, j);
            auto it = memo.find(key);
            if (it != memo.end()) return it->second;

            bool cl = (budget >= a - 1e-12L);
            bool ch = (budget >= b - 1e-12L);
            long long val;
            if (!cl && !ch) {
                val = 1;
            } else if (cl && ch) {
                long long fl = f(i + 1, j);
                long long fh = f(i, j + 1);
                val = min((long long)2e18, fl + fh + 1);
            } else if (cl) {
                val = f(i + 1, j) + 1;
            } else {
                val = f(i, j + 1) + 1;
            }

            memo[key] = val;
            return val;
        };

        long long fval = f(0, 0);
        if (fval >= n)
            hi = mid;
        else
            lo = mid;
    }

    return hi;
}

int main() {
    ios_base::sync_with_stdio(false);

    vector<long long> fib(31);
    fib[1] = 1; fib[2] = 1;
    for (int i = 3; i <= 30; i++)
        fib[i] = fib[i-1] + fib[i-2];

    long long N = 1000000000000LL; // 10^12

    long double total = 0.0L;
    for (int k = 1; k <= 30; k++) {
        long double a = sqrtl((long double)k);
        long double b = sqrtl((long double)fib[k]);
        long double c = solve(N, a, b);
        total += c;
    }

    printf("%.8Lf\n", total);

    return 0;
}
