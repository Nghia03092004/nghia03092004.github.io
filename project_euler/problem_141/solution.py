"""
Problem 141: Investigating Progressive Numbers
Find the sum of all progressive perfect squares below 10^12.

n is progressive if n = q*d + r where q, d, r form a geometric sequence
with q > d > r >= 0.

Parametrization: n = a^3*b*c^2 + b^2*c with gcd(a,b)=1, a>b>=1, c>=1.
"""

from math import gcd, isqrt

def solve():
    LIMIT = 10**12
    progressive_squares = set()

    a = 2
    while a * a * a < LIMIT:
        for b in range(1, a):
            if gcd(a, b) != 1:
                continue
            a3b = a**3 * b
            if a3b >= LIMIT:
                break
            c = 1
            while True:
                n = a3b * c * c + b * b * c
                if n >= LIMIT:
                    break
                s = isqrt(n)
                if s * s == n:
                    progressive_squares.add(n)
                c += 1
        a += 1

    answer = sum(progressive_squares)
    print(answer)
