"""
Project Euler Problem 192: Best Approximations

For each non-square d <= 100000, find the denominator of the best
approximation to sqrt(d) with denominator bound 10^12, then sum them.
"""

import math


def best_denominator(d, bound):
    a0 = math.isqrt(d)
    if a0 * a0 == d:
        return 0

    m = 0
    den = 1
    a = a0
    q_prev2, q_prev1 = 1, 0

    while True:
        q_curr = a * q_prev1 + q_prev2
        if q_curr > bound:
            t = (bound - q_prev2) // q_prev1
            if t <= 0:
                return q_prev1

            # Compare the last admissible semiconvergent with the previous convergent
            # using the complete quotient alpha = (sqrt(d) + m) / den.
            rhs = den * (2 * t * q_prev1 + q_prev2) - m * q_prev1
            if rhs * rhs > d * q_prev1 * q_prev1:
                return t * q_prev1 + q_prev2
            return q_prev1

        q_prev2, q_prev1 = q_prev1, q_curr
        m = a * den - m
        den = (d - m * m) // den
        a = (a0 + m) // den


def solve(limit=100000, bound=10**12):
    total = 0
    for d in range(2, limit + 1):
        total += best_denominator(d, bound)
    return total


if __name__ == "__main__":
    answer = solve()
    assert answer == 57060635927998347
    print(answer)
