"""
Problem 729: Range of Periodic Sequence

Recurrence: a_{n+1} = a_n - 1/a_n
Substitution a = cot(theta) linearizes to theta -> 2*theta (mod pi).
Period-p orbits correspond to m*pi/(2^p-1) under the doubling map.
"""

import numpy as np
from math import gcd

def compute_S(P):
    """Compute S(P) = sum of ranges of all periodic sequences with period <= P."""
    total = 0.0
    # For each possible period p, find all orbits of the doubling map mod (2^p - 1)
    # But we must only count orbits of EXACT period p (not divisors of p)
    all_visited = set()  # (p, m) pairs already counted

    for p in range(1, P + 1):
        N = (1 << p) - 1  # 2^p - 1
        visited = [False] * N
        for m in range(1, N):
            if visited[m]:
                continue
            # Trace orbit
            orbit_indices = []
            x = m
            while not visited[x]:
                visited[x] = True
                orbit_indices.append(x)
                x = (2 * x) % N

            # Check exact period
            period = len(orbit_indices)
            if period != p:
                continue  # this orbit has a smaller period, already counted

            # Compute cot values
            cot_vals = [1.0 / np.tan(idx * np.pi / N) for idx in orbit_indices]
            rng = max(cot_vals) - min(cot_vals)
            total += rng

    return total

# Verify
S2 = compute_S(2)
print(f"S(2) = {S2:.4f} (expected {2*np.sqrt(2):.4f})")

S3 = compute_S(3)
print(f"S(3) = {S3:.4f} (expected ~14.6461)")

S5 = compute_S(5)
print(f"S(5) = {S5:.4f} (expected ~124.1056)")

# Full answer
S25 = compute_S(25)
print(f"S(25) = {S25:.4f}")
