#!/usr/bin/env python3
"""
Problem 279: Triangles with Integral Sides and an Integral Angle

Count triangles with integer sides, perimeter <= 10^8, and at least one
angle that is an integer number of degrees (60, 90, or 120 by Niven's theorem).

Four families (no overlaps):
  1. Equilateral (all 60-deg): a=b=c, perimeter 3a
  2. 120-deg: c^2 = a^2+ab+b^2, parametrized via Eisenstein integers
  3. 60-deg (non-equilateral): b^2 = a^2+c^2-ac, two sub-cases
  4. 90-deg (Pythagorean): c^2 = a^2+b^2, Euclid parametrization
"""

from math import gcd, isqrt

def solve(N=10**8):
    count = 0

    # Equilateral
    count += N // 3

    # 120-degree: perimeter = (2m+n)(m+n)
    m = 2
    while True:
        min_p = (2*m+1)*(m+1)
        if min_p > N:
            break
        for n in range(1, m):
            if gcd(m, n) != 1:
                continue
            if (m - n) % 3 == 0:
                continue
            p0 = (2*m+n)*(m+n)
            if p0 > N:
                break
            count += N // p0
        m += 1

    # 60-degree Case 1: perimeter = 3m(m+n)
    m = 2
    while True:
        min_p = 3*m*(m+1)
        if min_p > N:
            break
        for n in range(1, m):
            if gcd(m, n) != 1:
                continue
            if (m - n) % 3 == 0:
                continue
            p0 = 3*m*(m+n)
            if p0 > N:
                break
            count += N // p0
        m += 1

    # 60-degree Case 2: perimeter = (2m+n)(m+2n)
    m = 2
    while True:
        min_p = (2*m+1)*(m+2)
        if min_p > N:
            break
        for n in range(1, m):
            if gcd(m, n) != 1:
                continue
            if (m - n) % 3 == 0:
                continue
            p0 = (2*m+n)*(m+2*n)
            if p0 > N:
                break
            count += N // p0
        m += 1

    # 90-degree (Pythagorean): perimeter = 2m(m+n)
    m = 2
    while True:
        min_p = 2*m*(m+1)
        if min_p > N:
            break
        for n in range(1, m):
            if gcd(m, n) != 1:
                continue
            if (m - n) % 2 == 0:
                continue
            p0 = 2*m*(m+n)
            if p0 > N:
                break
            count += N // p0
        m += 1

    return count

if __name__ == "__main__":
    N = 10**8
    answer = solve(N)
    print(answer)
