"""
Problem 174: Counting the Number of "Hollow" Square Laminae
That Can Form One, Two, Three, ... Distinct Arrangements

N(t) = number of laminae with exactly t tiles.
Count values of t <= 10^6 with 1 <= N(t) <= 10.

Key insight: t = 4m, and N(t) = number of divisors k of m with k < sqrt(m).
"""

import math

def solve():
    LIMIT = 1_000_000
    M = LIMIT // 4  # 250000

    N = [0] * (M + 1)

    # Sieve: for each k, find all q > k with k*q <= M
    k = 1
    while k * k < M:
        q = k + 1
        while k * q <= M:
            N[k * q] += 1
            q += 1
        k += 1

    ans = sum(1 for m in range(1, M + 1) if 1 <= N[m] <= 10)
    print(ans)

if __name__ == "__main__":
    solve()
