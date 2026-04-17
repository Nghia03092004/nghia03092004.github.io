"""
Problem 173: Hollow Square Laminae

Count distinct hollow square laminae using at most N = 10^6 tiles.
Lamina (a, b): outer side a, inner side b, same parity, tiles = a^2 - b^2.
"""

import math

def solve():
    N = 1_000_000
    total = 0
    a = 3
    while 4 * (a - 1) <= N:
        b_max = a - 2
        lo = a * a - N
        if lo <= 1:
            b_min = 2 if a % 2 == 0 else 1
        else:
            b_min = math.isqrt(lo - 1) + 1
            while b_min * b_min < lo:
                b_min += 1
            if b_min % 2 != a % 2:
                b_min += 1
        if b_min <= b_max:
            total += (b_max - b_min) // 2 + 1
        a += 1
    print(total)

if __name__ == "__main__":
    solve()
