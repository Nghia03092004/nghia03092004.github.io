"""
Project Euler Problem 398: Cutting Rope

A rope of length n has n-1 integer points. We choose m-1 of them at random
and cut to create m segments. E(n, m) = expected length of 2nd shortest segment.

Find E(10^7, 100) rounded to 5 decimal places.

Answer: 2010.59096

Approach:
  E[2nd min] = sum_{t>=1} P(2nd min >= t)
  P(2nd min >= t) = P(all parts >= t) + P(exactly one part < t)
  Both terms are computed via binomial coefficients (stars-and-bars) with
  the hockey stick identity used to collapse the inner sum to O(1).
"""

from math import comb, lgamma, exp
from decimal import Decimal, getcontext
import time

def E_exact(n: int, m: int) -> Decimal:
    """
    Compute E(n, m) using exact integer arithmetic.
    Returns a Decimal with high precision.
    """
    getcontext().prec = 50
    total_comps = comb(n - 1, m - 1)
    t_max = (n - 1) // (m - 1) + 1
    numerator = 0

    for t in range(1, t_max + 1):
        # Count compositions with ALL parts >= t
        rem_all = n - m * (t - 1)
        p_all = comb(rem_all - 1, m - 1) if rem_all >= m else 0

        # Count compositions with EXACTLY ONE part < t
        R = n - (m - 1) * t
        S = min(t - 1, R)
        p_one = 0
        if S >= 1 and R >= 1:
            upper = comb(R + m - 2, m - 1)
            lower = comb(R - S + m - 2, m - 1)
            p_one = m * (upper - lower)

        contrib = p_all + p_one
        if contrib == 0:
            break
        numerator += contrib

    return Decimal(numerator) / Decimal(total_comps)

def E_float(n: int, m: int) -> float:
    """
    Compute E(n, m) using log-gamma for speed (slight precision loss).
    Suitable for plotting and exploration.
    """
    def log_comb(a, b):
        if b < 0 or a < b:
            return float('-inf')
        if b == 0 or a == b:
            return 0.0
        return lgamma(a + 1) - lgamma(b + 1) - lgamma(a - b + 1)

    log_total = log_comb(n - 1, m - 1)
    result = 0.0
    t_max = (n - 1) // (m - 1) + 1

    for t in range(1, t_max + 1):
        rem_all = n - m * (t - 1)
        p_all = exp(log_comb(rem_all - 1, m - 1) - log_total) if rem_all >= m else 0.0

        R = n - (m - 1) * t
        S = min(t - 1, R)
        p_one = 0.0
        if S >= 1 and R >= 1:
            log_upper = log_comb(R + m - 2, m - 1) - log_total
            log_lower = log_comb(R - S + m - 2, m - 1) - log_total
            p_one = m * (exp(log_upper) - exp(log_lower))

        prob = p_all + p_one
        if prob < 1e-20 and t > 2:
            break
        result += prob

    return result

def verify_examples():
    """Verify against the known examples in the problem statement."""
    from fractions import Fraction

    # E(3, 2) = 2
    total = comb(2, 1)
    num = 0
    for t in range(1, 4):
        rem_all = 3 - 2 * (t - 1)
        p_all = comb(rem_all - 1, 1) if rem_all >= 2 else 0
        R = 3 - t
        S = min(t - 1, R)
        p_one = 0
        if S >= 1 and R >= 1:
            p_one = 2 * (comb(R, 1) - comb(R - S, 1))
        contrib = p_all + p_one
        if contrib == 0:
            break
        num += contrib
    assert Fraction(num, total) == 2, f"E(3,2) failed: got {Fraction(num, total)}"

    # E(8, 3) = 16/7
    result = E_exact(8, 3)
    assert abs(float(result) - 16 / 7) < 1e-30, f"E(8,3) failed: got {result}"

    print("All examples verified.")

def solve():
    """Solve the main problem: E(10^7, 100)."""
    n, m = 10**7, 100
    start = time.time()
    result = E_exact(n, m)
    elapsed = time.time() - start
    answer = f"{float(result):.5f}"
    print(f"E(10^7, 100) = {result}")
    print(f"Rounded to 5 decimal places: {answer}")
    print(f"Computation time: {elapsed:.3f}s")
    return float(result)

def create_visualization():
    """Create plots exploring the behavior of E(n, m)."""
