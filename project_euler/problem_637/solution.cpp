#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 637: Flexible Digit Sum
 *
 * f(n,B) = number of digit-sum steps to reach a single digit in base B.
 * g(n,B1,B2) = sum of i <= n where f(i,B1) == f(i,B2).
 * Find g(10^7, 10, 3).
 */

int digitSum(int n, int base) {
    int s = 0;
    while (n > 0) {
        s += n % base;
        n /= base;
    }
    return s;
}

int f(int n, int base) {
    int steps = 0;
    while (n >= base) {
        n = digitSum(n, base);
        steps++;
    }
    return steps;
}

int main() {
    const int N = 10000000;
    long long result = 0;

    for (int i = 1; i <= N; i++) {
        if (f(i, 10) == f(i, 3)) {
            result += i;
        }
    }

    cout << result << endl;
    return 0;
}
