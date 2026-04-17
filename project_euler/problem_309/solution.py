"""
Problem 309: Integer Ladders

Two ladders of integer lengths x, y lean against opposite walls of an alley
of integer width w, crossing at integer height h.
x^2 = w^2 + p^2, y^2 = w^2 + q^2, h = pq/(p+q).

Count (x, y, h) triplets with 0 < x < y < 1000000.

For h to be integer: let g=gcd(p,q), p=g*alpha, q=g*beta, gcd(alpha,beta)=1.
Then h = g*alpha*beta/(alpha+beta), integer iff (alpha+beta) | g.

Answer: 210139
"""

from math import gcd
from collections import defaultdict

def solve():
    LIMIT = 1000000

    # For each w, store (other_leg, hypotenuse) pairs
    by_w = defaultdict(list)

    # Generate primitive Pythagorean triples and their multiples
    m = 2
    while m * m < LIMIT:
        for n in range(1, m):
            if (m - n) % 2 == 0:
                continue
            if gcd(m, n) != 1:
                continue

            w1 = m * m - n * n
            p1 = 2 * m * n
            hyp = m * m + n * n

            d = 1
            while d * hyp < LIMIT:
                by_w[d * w1].append((d * p1, d * hyp))
                by_w[d * p1].append((d * w1, d * hyp))
                d += 1

        m += 1

    answer = 0
    for w, legs in by_w.items():
        sz = len(legs)
        if sz < 2:
            continue
        for i in range(sz):
            for j in range(i + 1, sz):
                p1, hyp1 = legs[i]
                p2, hyp2 = legs[j]
                if hyp1 == hyp2:
                    continue  # x < y strictly
                g = gcd(p1, p2)
                alpha = p1 // g
                beta = p2 // g
                if g % (alpha + beta) == 0:
                    answer += 1

    print(answer)
