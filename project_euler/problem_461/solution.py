#!/usr/bin/env python3
"""Project Euler Problem 461: Almost Pi

Find g(10000) where g(n) = a^2+b^2+c^2+d^2 for the quadruple (a,b,c,d)
minimizing |f_n(a)+f_n(b)+f_n(c)+f_n(d) - pi|, with f_n(k) = e^(k/n) - 1.

Meet-in-the-middle: enumerate pair sums, sort, binary search for complement.
Answer: 159820276
"""

import math
import bisect


def solve(n):
    PI = math.pi
    k_max = int(n * math.log(PI + 1)) + 1
    fvals = [math.exp(k / n) - 1.0 for k in range(k_max + 1)]

    # Generate all pair sums f_n(a) + f_n(b) with a <= b and sum <= pi
    pairs = []
    for a in range(k_max + 1):
        if fvals[a] > PI:
            break
        for b in range(a, k_max + 1):
            s = fvals[a] + fvals[b]
            if s > PI:
                break
            pairs.append((s, a, b))
    pairs.sort()
    pair_sums = [p[0] for p in pairs]

    best_error = float('inf')
    best_abcd = (0, 0, 0, 0)
    for s_ab, a, b in pairs:
        target = PI - s_ab
        idx = bisect.bisect_left(pair_sums, target)
        for j in (idx - 1, idx):
            if 0 <= j < len(pairs):
                error = abs(s_ab + pairs[j][0] - PI)
                if error < best_error:
                    best_error = error
                    best_abcd = (a, b, pairs[j][1], pairs[j][2])

    a, b, c, d = best_abcd
    return a * a + b * b + c * c + d * d


if __name__ == "__main__":
    print(solve(10000))
