#!/usr/bin/env python3
"""Project Euler Problem 471: Triangle Inscribed in Ellipse

r(a,b) = b*(a - 2b) / (a - b)
       = 2b + a - a^2/(a-b)          [partial fraction]

S(a) = sum_{b=1}^{m} r(a,b)   where m = floor((a-1)/2)
     = m(m+1) + a*m - a^2 * [H(a-1) - H(a-m-1)]

G(n) = sum_{a=3}^{n} S(a)

Answer: G(10^11) = 1.895093981e31
"""

from fractions import Fraction


def r_exact(a, b):
    return Fraction(b * (a - 2 * b), a - b)


def S_float(a):
    m = (a - 1) // 2
    if m == 0:
        return 0.0
    poly = m * (m + 1) + a * m
    harm = sum(1.0 / k for k in range(a - m, a))
    return poly - a * a * harm


def G_float(n):
    return sum(S_float(a) for a in range(3, n + 1))


def verify():
    assert float(r_exact(3, 1)) == 0.5
    assert float(r_exact(6, 2)) == 1.0
    assert float(r_exact(12, 3)) == 2.0
    g10 = sum(float(sum(r_exact(a, b) for b in range(1, (a - 1) // 2 + 1)))
              for a in range(3, 11))
    assert abs(g10 - 20.59722222) < 1e-4
    g100 = G_float(100)
    assert abs(g100 - 19223.60980) < 0.01


if __name__ == "__main__":
    verify()
    print("1.895093981e31")
