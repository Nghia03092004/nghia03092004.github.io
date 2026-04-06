"""
Project Euler Problem 75: Singular Integer Right Triangles

Count values of L <= 1,500,000 for which exactly one integer-sided
right triangle can be formed with perimeter L.

By Theorems 1-3, generate all primitive Pythagorean triple perimeters
via Euclid's parametrization L0 = 2m(m+n), then count multiples.
A perimeter is "singular" iff it is hit exactly once.
"""

from math import gcd, isqrt


def solve():
    LMAX = 1_500_000
    cnt = [0] * (LMAX + 1)

    mlimit = isqrt(LMAX // 2) + 1

    for m in range(2, mlimit + 1):
        for n in range(1, m):
            if (m - n) % 2 == 0:
                continue
            if gcd(m, n) != 1:
                continue
            L0 = 2 * m * (m + n)
            if L0 > LMAX:
                break
            for L in range(L0, LMAX + 1, L0):
                cnt[L] += 1

    return sum(1 for L in range(1, LMAX + 1) if cnt[L] == 1)


if __name__ == "__main__":
    answer = solve()
assert answer == 161667
print(answer)
