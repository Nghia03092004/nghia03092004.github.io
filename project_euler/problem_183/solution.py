"""
Project Euler Problem 183: Maximum Product of Parts

For each N from 5 to 10000, find k maximising (N/k)^k.  Then D(N) = -N if
the maximum is a terminating decimal, else +N.  Compute sum of D(N).
"""

import math


def is_terminating(n, k):
    """Check whether n/k in lowest terms has denominator with only factors 2 and 5."""
    d = k // math.gcd(n, k)
    while d % 2 == 0:
        d //= 2
    while d % 5 == 0:
        d //= 5
    return d == 1


def solve():
    E = math.e
    total = 0

    for N in range(5, 10001):
        k1 = int(N / E)
        k2 = k1 + 1
        if k1 < 1:
            k1 = 1

        v1 = k1 * math.log(N / k1)
        v2 = k2 * math.log(N / k2)
        k = k1 if v1 >= v2 else k2

        if is_terminating(N, k):
            total -= N
        else:
            total += N

    return total


if __name__ == "__main__":
    answer = solve()
assert answer == 48861552
print(answer)
