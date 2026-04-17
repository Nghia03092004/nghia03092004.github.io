"""
Project Euler Problem 66: Diophantine Equation (Pell's Equation)

Find the value of D <= 1000 for which the minimal solution x of
x^2 - D*y^2 = 1 is maximized.

Uses continued fraction expansion of sqrt(D) to find fundamental solutions.
"""

from math import isqrt

def solve_pell(D):
    """
    Solve x^2 - D*y^2 = 1 (Pell's equation) for the fundamental solution.
    Returns the minimal positive x using continued fraction expansion of sqrt(D).
    """
    a0 = isqrt(D)
    if a0 * a0 == D:
        return None  # D is a perfect square, no solution

    # Find the period length of the continued fraction of sqrt(D)
    m, d, a = 0, 1, a0
    period = 0
    while True:
        m = d * a - m
        d = (D - m * m) // d
        a = (a0 + m) // d
        period += 1
        if a == 2 * a0:
            break

    # If period is even, solution is at convergent index (period - 1)
    # If period is odd, solution is at convergent index (2*period - 1)
    target = period - 1 if period % 2 == 0 else 2 * period - 1

    # Compute convergent p_{target}
    m, d, a = 0, 1, a0
    p_prev, p_curr = 1, a0  # p_{-1} = 1, p_0 = a0

    if target == 0:
        return p_curr

    for i in range(1, target + 1):
        m = d * a - m
        d = (D - m * m) // d
        a = (a0 + m) // d

        p_prev, p_curr = p_curr, a * p_curr + p_prev

    return p_curr

def main():
    max_x = 0
    best_D = 0

    for D in range(2, 1001):
        x = solve_pell(D)
        if x is not None and x > max_x:
            max_x = x
            best_D = D

    print(best_D)

if __name__ == "__main__":
    main()
