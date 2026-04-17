"""
Problem 64: Odd Period Square Roots

How many continued fractions for sqrt(N), N <= 10000, have an odd period?
"""

import math


def cf_period(N):
    """Return the period of the continued fraction of sqrt(N).
    Returns 0 if N is a perfect square.
    """
    a0 = int(math.isqrt(N))
    if a0 * a0 == N:
        return 0

    m, d, a = 0, 1, a0
    period = 0
    while True:
        m = d * a - m
        d = (N - m * m) // d
        a = (a0 + m) // d
        period += 1
        if a == 2 * a0:
            return period


def solve():
    count = 0
    for N in range(2, 10001):
        p = cf_period(N)
        if p > 0 and p % 2 == 1:
            count += 1
    return count


answer = solve()
assert answer == 1322
print(answer)
