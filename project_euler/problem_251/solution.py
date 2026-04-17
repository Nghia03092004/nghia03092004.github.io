"""
Project Euler Problem 251: Cardano Triplets

A Cardano Triplet (a, b, c) satisfies:
    cbrt(a + b*sqrt(c)) + cbrt(a - b*sqrt(c)) = 1

This reduces to (a+1)^2 * (8a-1) = 27 * b^2 * c.
Setting a = 3m-1 yields m^2 * (8m-3) = b^2 * c.

Parametrize: b = e*f, m = e*r with gcd(f,r) = 1 and f^2 | (8m-3).
Then c = r^2*(8m-3)/f^2, and a+b+c = (3m-1) + e*f + r^2*(8m-3)/f^2 <= N.

For each (r, f) pair, valid e values form an arithmetic progression mod f^2.

Answer: 18946051
"""

from math import gcd


def solve():
    N = 110_000_000
    count = 0

    r = 1
    while r * (r + 3) <= N:
        # f = 1 case (closed form)
        coeff = 3 * r + 1 + 8 * r * r * r
        offset = 3 * r * r + 1
        if coeff <= N + offset:
            emax = (N + offset) // coeff
            if emax >= 1:
                count += emax

        # f >= 3, odd, gcd(f, r) = 1
        f = 3
        while True:
            if f * f * f > 8 * r * N:
                break
            if gcd(f, r) != 1:
                f += 2
                continue
            f2 = f * f
            val = (8 * r) % f2
            if gcd(val, f2) != 1:
                f += 2
                continue

            inv_val = pow(val, -1, f2)
            e0 = (3 * inv_val) % f2
            if e0 == 0:
                e0 = f2

            m = r * e0
            h = 8 * m - 3
            s = (3 * m - 1) + e0 * f + r * r * (h // f2)
            if s <= N:
                delta = f2 * (3 * r + f) + 8 * r * r * r
                kmax = (N - s) // delta
                count += kmax + 1

            f += 2
        r += 1

    print(count)


if __name__ == "__main__":
    solve()
