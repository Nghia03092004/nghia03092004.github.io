"""
Problem 86: Cuboid Route

Find least M such that the number of cuboids a <= b <= c <= M
with integer shortest surface path exceeds 1,000,000.

Shortest path for a <= b <= c: sqrt((a+b)^2 + c^2)
Let s = a+b; need s^2 + c^2 = perfect square.
Count of valid (a,b) for given (s,c): floor(s/2) - max(1, s-c) + 1.

Answer: 1818
"""

import math


def solve():
    total = 0
    M = 0

    while total <= 1_000_000:
        M += 1
        c = M
        for s in range(2, 2 * c + 1):
            dsq = s * s + c * c
            d = math.isqrt(dsq)
            if d * d == dsq:
                lo = max(1, s - c)
                hi = s // 2
                if hi >= lo:
                    total += hi - lo + 1

    print(M)


if __name__ == "__main__":
    solve()
