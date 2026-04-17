"""
Problem 513: Integral Median
A triangle with integer sides a <= b <= c has median to side a:
    m_a = (1/2) * sqrt(2b^2 + 2c^2 - a^2)
Count triangles with a <= b <= c <= N where m_a is a positive integer.
"""

import math

def solve(N: int):
    """
    Count triangles (a, b, c) with a <= b <= c <= N where
    m_a = (1/2)*sqrt(2b^2 + 2c^2 - a^2) is a positive integer.

    We need 2b^2 + 2c^2 - a^2 = 4*m^2 for some positive integer m.
    Equivalently: 2b^2 + 2c^2 - a^2 must be a perfect square of an even number.
    """
    count = 0
    for b in range(1, N + 1):
        for c in range(b, N + 1):
            val_base = 2 * b * b + 2 * c * c
            # a ranges from 1 to b, and triangle inequality: a + b > c => a > c - b
            a_min = max(1, c - b + 1)
            a_max = b
            for a in range(a_min, a_max + 1):
                val = val_base - a * a
                if val <= 0:
                    continue
                sq = math.isqrt(val)
                if sq * sq == val and sq % 2 == 0:
                    count += 1
    return count

def solve_brute_force(N: int):
    """Direct brute-force for small N verification."""
    count = 0
    for a in range(1, N + 1):
        for b in range(a, N + 1):
            for c in range(b, N + 1):
                if a + b <= c:
                    continue
                val = 2 * b * b + 2 * c * c - a * a
                if val > 0:
                    sq = math.isqrt(val)
                    if sq * sq == val and sq % 2 == 0:
                        count += 1
    return count

# Verify on small inputs
N_small = 50
ans1 = solve(N_small)
ans2 = solve_brute_force(N_small)
assert ans1 == ans2, f"Mismatch: {ans1} vs {ans2}"
print(f"T({N_small}) = {ans1}")

# Show growth for moderate N values
Ns = list(range(10, 201, 10))
counts = [solve(n) for n in Ns]
for n, c in zip(Ns, counts):
    print(f"T({n}) = {c}")
