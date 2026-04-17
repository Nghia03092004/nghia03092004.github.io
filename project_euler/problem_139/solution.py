"""
Problem 139: Pythagorean Tiles

Count Pythagorean triples (a, b, c) with perimeter <= 10^8 where
c % |b - a| == 0 (the tiling condition).

Enumerate primitive triples via (m, n) parameterization and count multiples.
"""

import math

def solve():
    P = 100_000_000
    count = 0

    m = 2
    while 2 * m * (m + 1) <= P:
        for n in range(1, m):
            if (m - n) % 2 == 0:
                continue
            if math.gcd(m, n) != 1:
                continue

            a = m * m - n * n
            b = 2 * m * n
            c = m * m + n * n
            perim = a + b + c

            if perim > P:
                break

            diff = abs(b - a)
            if diff > 0 and c % diff == 0:
                count += P // perim
        m += 1

    return count

if __name__ == "__main__":
    answer = solve()
assert answer == 10057761
print(answer)
