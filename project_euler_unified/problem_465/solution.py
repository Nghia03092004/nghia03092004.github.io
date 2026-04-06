"""
Project Euler Problem 465: Polar Polygons

A polar polygon has the origin strictly inside its kernel.
Vertices are lattice points with |x|, |y| <= n.

P(n) = number of polar polygons.
Given: P(1)=131, P(2)=1648531, P(3)=1099461296175, P(343) mod 10^9+7 = 937293740.
Find: P(713) mod 10^9+7.

KEY INSIGHT:
A polygon is star-shaped wrt origin iff all consecutive cross products v_i x v_{i+1}
have the same sign. This means vertices are in strict angular order around origin.

A polar polygon is determined by choosing a subset of ray directions (>= 3) with one
lattice point per ray, such that max angular gap < pi.

P(n) = AllWeighted(>=3) - Bad(>=3)

AllWeighted(>=3) = prod(1+c(r)) - 1 - S1 - e2
Bad(>=3) uses the anchor trick with two-pointer sweep.
"""

import sys
import math
from math import gcd, atan2

MOD_PRIME = 10**9 + 7

def modinv(a, m=MOD_PRIME):
    return pow(a, m - 2, m)

def solve(n, mod=MOD_PRIME):
    # Step 1: Enumerate primitive ray directions and their counts
    rays = []
    for a in range(-n, n + 1):
        for b in range(-n, n + 1):
            if a == 0 and b == 0:
                continue
            if gcd(abs(a), abs(b)) != 1:
                continue
            count = n // max(abs(a), abs(b))
            angle = atan2(b, a)
            if angle < 0:
                angle += 2 * math.pi
            rays.append((angle, count))

    rays.sort()
    R = len(rays)
    counts = [r[1] for r in rays]
    angles = [r[0] for r in rays]

    # AllWeighted(>=3) = prod(1+c) - 1 - S1 - e2
    prod_all = 1
    S1 = 0
    S2 = 0
    for c in counts:
        prod_all = prod_all * (1 + c) % mod
        S1 = (S1 + c) % mod
        S2 = (S2 + c * c) % mod
    inv2 = modinv(2, mod)
    e2 = (S1 * S1 - S2) % mod * inv2 % mod
    all_ge3 = (prod_all - 1 - S1 - e2) % mod

    # Bad(>=3): anchor trick with two-pointer
    # For each anchor i, window = rays j in (angle[i], angle[i]+pi] in doubled array.
    # Bad(>=3) += c(i) * [Pi(i) - 1 - Sigma(i)]

    # Doubled arrays for circular handling
    angles2 = angles + [a + 2 * math.pi for a in angles]
    counts2 = counts + counts
    N2 = 2 * R

    # Precompute prefix products, their inverses, and prefix sums
    prefix_prod = [1] * (N2 + 1)
    prefix_sum = [0] * (N2 + 1)
    for k in range(N2):
        prefix_prod[k + 1] = prefix_prod[k] * (1 + counts2[k]) % mod
        prefix_sum[k + 1] = (prefix_sum[k] + counts2[k]) % mod

    # Precompute inverse prefix products
    inv_prefix_prod = [1] * (N2 + 1)
    inv_prefix_prod[N2] = modinv(prefix_prod[N2], mod)
    for k in range(N2 - 1, -1, -1):
        inv_prefix_prod[k] = inv_prefix_prod[k + 1] * (1 + counts2[k]) % mod

    def range_prod(l, r):
        """Product of (1+counts2[k]) for k in [l, r)."""
        if l >= r:
            return 1
        return prefix_prod[r] * inv_prefix_prod[l] % mod

    def range_sum(l, r):
        """Sum of counts2[k] for k in [l, r)."""
        if l >= r:
            return 0
        return (prefix_sum[r] - prefix_sum[l]) % mod

    bad_ge3 = 0

    # Two-pointer sweep
    j_ptr = 0
    for i in range(R):
        target = angles[i] + math.pi + 1e-12  # angle[i] + pi, with small eps for float
        if j_ptr <= i:
            j_ptr = i + 1
        while j_ptr < i + R and angles2[j_ptr] < target:
            j_ptr += 1
        result_j = j_ptr - 1

        if result_j <= i:
            continue

        Pi_i = range_prod(i + 1, result_j + 1)
        Sigma_i = range_sum(i + 1, result_j + 1)

        bad_ge3 = (bad_ge3 + counts[i] * ((Pi_i - 1 - Sigma_i) % mod)) % mod

    P_n = (all_ge3 - bad_ge3) % mod
    return P_n

def create_visualization(n=2):
    """Visualize ray directions and counts."""
    rays = []
    for a in range(-n, n + 1):
        for b in range(-n, n + 1):
            if a == 0 and b == 0:
                continue
            if gcd(abs(a), abs(b)) != 1:
                continue
            count = n // max(abs(a), abs(b))
            angle = atan2(b, a)
            rays.append((a, b, count, angle))
