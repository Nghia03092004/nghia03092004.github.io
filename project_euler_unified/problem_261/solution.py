"""
Problem 261: Pivotal Square Sums

Find the sum of all distinct square-pivots k <= 10^10.

Reduction: the pivot equation reduces to a generalized Pell equation
X^2 - D*Y^2 = g where D = s*g, with m = s*p^2 (s squarefree) and
m+1 = g*r^2 (g squarefree). Iterating Pell solutions recovers k values.
"""

import math

LIMIT = 10**10


def build_spf(n):
    """Smallest prime factor sieve up to n."""
    spf = list(range(n + 1))
    for i in range(2, int(math.isqrt(n)) + 1):
        if spf[i] == i:
            for j in range(i * i, n + 1, i):
                if spf[j] == j:
                    spf[j] = i
    return spf


def squarefree_and_sqrt(n, spf):
    """Decompose n = sf * sq^2 with sf squarefree."""
    sf, sq = 1, 1
    while n > 1:
        p = spf[n]
        e = 0
        while n % p == 0:
            n //= p
            e += 1
        if e & 1:
            sf *= p
        sq *= p ** (e // 2)
    return sf, sq


def pell_fundamental(D, cache):
    """Fundamental solution (x, y) to x^2 - D*y^2 = 1 via continued fractions."""
    if D in cache:
        return cache[D]
    a0 = int(math.isqrt(D))
    m, d, a = 0, 1, a0
    num1, num = 1, a
    den1, den = 0, 1
    while num * num - D * den * den != 1:
        m = d * a - m
        d = (D - m * m) // d
        a = (a0 + m) // d
        num2, num1 = num1, num
        den2, den1 = den1, den
        num = a * num1 + num2
        den = a * den1 + den2
    cache[D] = (num, den)
    return num, den


def solve(limit=LIMIT):
    mmax = (math.isqrt(1 + 2 * limit) - 1) // 2
    spf = build_spf(mmax + 2)
    pell_cache = {}
    pivots = set()

    for m in range(1, mmax + 1):
        s, p = squarefree_and_sqrt(m, spf)
        g, r = squarefree_and_sqrt(m + 1, spf)
        D = s * g
        x1, y1 = pell_fundamental(D, pell_cache)
        X, Y = g * r, p

        while True:
            q = Y
            numerator = s * p * (p + q)
            if numerator // 2 > limit and q > p:
                break
            if numerator % 2 == 0:
                k = numerator // 2
                if k <= limit and k >= 2 * m * (m + 1):
                    u = X // g
                    t = g * r * u
                    if (t - m - 1) % 2 == 0:
                        n = (t - m - 1) // 2
                        if n >= k:
                            pivots.add(k)
            X, Y = X * x1 + Y * y1 * D, X * y1 + Y * x1

    return sum(pivots)


if __name__ == "__main__":
    answer = solve()
assert answer == 238890850232021
print(answer)
