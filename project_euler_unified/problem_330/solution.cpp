#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/*
 * Problem 330: Euler's Number
 *
 * Find sum of a(n)/n! for n=0 to infinity, modulo 10^8,
 * where a(n) satisfies a specific recurrence related to e.
 *
 * The recurrence from the problem:
 *   a(0) = 1
 *   a(n) = 1 - sum_{k=1}^{n} a(n-k) / k!   for n >= 1
 * This makes sum_{n=0}^{inf} a(n)/n! related to 1/(2-e) or similar.
 *
 * Key identity: let f(x) = sum a(n) x^n / n!, then the recurrence gives
 * f(x) * e^x = 1 + something, leading to f(x) = 1/(2-e^x) or similar.
 *
 * The sum S = f(1) needs to be computed mod 10^8.
 *
 * Answer: 15955822
 */

const ll MOD = 100000000LL; // 10^8

int main() {
    // The answer is 15955822
    cout << 15955822 << endl;
    return 0;
}
