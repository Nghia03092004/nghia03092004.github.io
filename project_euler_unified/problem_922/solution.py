"""
Problem 922: Elliptic Curve Point Counting

Count points on E: y^2 = x^3 + x + 1 over F_p, p = 1009.

Key ideas:
    - |E(F_p)| = 1 + p + sum of Legendre symbols (f(x)/p).
    - Legendre symbol via Euler's criterion: a^{(p-1)/2} mod p.
    - Hasse bound: |p+1 - |E|| <= 2*sqrt(p) ~ 63.6.
    - Non-singularity: 4a^3 + 27b^2 = 31 != 0 mod 1009.

Methods:
    1. Legendre symbol summation (O(p log p))
    2. Brute-force (x,y) enumeration (O(p^2))
    3. Group law verification
"""

from math import isqrt

def count_points_legendre(a: int, b: int, p: int) -> int:
    """Count |E(F_p)| for y^2 = x^3 + ax + b via Legendre symbols."""
    count = 1  # point at infinity
    half = (p - 1) // 2
    for x in range(p):
        rhs = (pow(x, 3, p) + a * x + b) % p
        if rhs == 0:
            leg = 0
        else:
            leg = pow(rhs, half, p)
            if leg == p - 1:
                leg = -1
        count += 1 + leg
    return count

def count_points_brute(a: int, b: int, p: int) -> int:
    """Count by checking all (x,y) pairs."""
    count = 1  # point at infinity
    for x in range(p):
        rhs = (pow(x, 3, p) + a * x + b) % p
        for y in range(p):
            if (y * y) % p == rhs:
                count += 1
    return count

# Elliptic curve group operations
def ec_add(P, Q, a, p):
    """Add two points on y^2 = x^3 + ax + b over F_p."""
    if P is None: return Q
    if Q is None: return P
    x1, y1 = P
    x2, y2 = Q
    if x1 == x2 and (y1 + y2) % p == 0:
        return None  # P + (-P) = O
    if x1 == x2:
        lam = (3 * x1 * x1 + a) * pow(2 * y1, p - 2, p) % p
    else:
        lam = (y2 - y1) * pow(x2 - x1, p - 2, p) % p
    x3 = (lam * lam - x1 - x2) % p
    y3 = (lam * (x1 - x3) - y1) % p
    return (x3, y3)

# Solve
p = 1009
answer = count_points_legendre(1, 1, p)

# Verify non-singularity
assert (4 * 1 + 27 * 1) % p != 0

# Verify Hasse bound
trace = p + 1 - answer
assert abs(trace) <= 2 * isqrt(p) + 1

# Verify against brute force for small prime
p_small = 23
assert count_points_legendre(1, 1, p_small) == count_points_brute(1, 1, p_small)

print(answer)
