"""
Problem 664: An Infinite Game (Conway's Soldiers variant)

F(n) = max squares a token can reach beyond the dividing line
with n rows of tokens.

Given: F(1)=6, F(2)=9, F(3)=13, F(11)=58, F(123)=1173.
Find F(1234567).
"""

import numpy as np
from math import log, sqrt, floor

# Golden ratio
PHI = (1 + sqrt(5)) / 2

# Known values for calibration
known = {1: 6, 2: 9, 3: 13, 11: 58, 123: 1173}

def potential_bound(n, d):
    """Total potential of n rows, trying to reach distance d."""
    # Sum over all tokens on the left side
    # Each row y=0..n-1, each column x=1,2,...
    # Weight of token at (-x, y) for target at (d, 0): phi^{-(x+d+y)}
    # = phi^{-d} * sum_{x=1}^inf phi^{-x} * sum_{y=0}^{n-1} phi^{-y}
    geom_x = 1.0 / (PHI - 1)  # sum phi^{-x} for x=1..inf
    geom_y = (1 - PHI**(-n)) / (1 - 1/PHI)  # sum phi^{-y} for y=0..n-1
    return PHI**(-d) * geom_x * geom_y

def F_upper_bound(n):
    """Upper bound on F(n) from potential function."""
    # Find max d such that potential_bound(n, d) >= 1
    lo, hi = 0, 100 * n
    while lo < hi:
        mid = (lo + hi + 1) // 2
        if potential_bound(n, mid) >= 1:
            lo = mid
        else:
            hi = mid - 1
    return lo

# Try to find the pattern
print("Potential-based upper bounds:")
for n in sorted(known.keys()):
    ub = F_upper_bound(n)
    print(f"  F({n}) = {known[n]}, upper bound = {ub}, ratio = {known[n]/n:.4f}")

# Linear regression on known values
ns = list(known.keys())
fs = [known[n] for n in ns]

# F(n) ≈ a*n + b
# Use least squares
import numpy as np
A_mat = np.array([[n, 1] for n in ns])
b_vec = np.array(fs)
result = np.linalg.lstsq(A_mat, b_vec, rcond=None)
a, b = result[0]
print(f"\nLinear fit: F(n) ≈ {a:.6f} * n + {b:.6f}")

# Check fit
for n in ns:
    pred = a * n + b
    print(f"  F({n}) = {known[n]}, predicted = {pred:.1f}")

# Compute F(1234567)
F_target = round(a * 1234567 + b)
print(f"\nF(1234567) ≈ {F_target}")
