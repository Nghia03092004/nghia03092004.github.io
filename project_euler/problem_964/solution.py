"""
Problem 964: Generalized Pentagonal Numbers

Sum of all generalized pentagonal numbers omega_k = k(3k-1)/2
for k = 0, +/-1, +/-2, ... that are below 10^7.

The generalized pentagonal numbers arise in Euler's pentagonal number
theorem for the partition function. The sequence begins:
    0, 1, 2, 5, 7, 12, 15, 22, 26, 35, 40, ...

Key results:
    - For positive k: omega_k = k(3k-1)/2
    - For negative k (use k -> -k): omega_{-k} = k(3k+1)/2
    - Count of generalized pentagonal numbers below 10^7: ~2582
    - Complexity: O(sqrt(N))

Methods:
    1. generalized_pentagonal     — generate all gen. pentagonal numbers < N
    2. is_pentagonal              — check if a number is pentagonal
    3. pentagonal_via_formula     — direct computation for given k range
    4. cumulative_sum_analysis    — growth analysis of cumulative sum
"""

import numpy as np

def generalized_pentagonal(N):
    """Generate all generalized pentagonal numbers below N, sorted."""
    result = []
    for k in range(1, N):
        val_pos = k * (3 * k - 1) // 2
        if val_pos >= N:
            break
        result.append(val_pos)
        val_neg = k * (3 * k + 1) // 2
        if val_neg < N:
            result.append(val_neg)
    result.sort()
    return result

def is_generalized_pentagonal(n):
    """Check if n is a generalized pentagonal number."""
    # n = k(3k-1)/2 => 24n+1 = (6k-1)^2
    # n = k(3k+1)/2 => 24n+1 = (6k+1)^2
    val = 24 * n + 1
    from math import isqrt
    s = isqrt(val)
    if s * s != val:
        return False
    # s = 6k-1 or s = 6k+1 => s % 6 == 5 or s % 6 == 1
    return s % 6 in (1, 5)

def pentagonal_for_k_range(k_min, k_max):
    """Compute omega_k for each k in [k_min, k_max]."""
    result = {}
    for k in range(k_min, k_max + 1):
        result[k] = k * (3 * k - 1) // 2
    return result

def cumulative_sum_growth(nums):
    """Return cumulative sum array."""
    return np.cumsum(nums)

# Verification
# Known first generalized pentagonal numbers
known = [1, 2, 5, 7, 12, 15, 22, 26, 35, 40]
gp_small = generalized_pentagonal(50)
assert gp_small == known, f"Expected {known}, got {gp_small}"

# Verify is_generalized_pentagonal
for v in known:
    assert is_generalized_pentagonal(v), f"{v} should be generalized pentagonal"

# Non-pentagonal checks
for v in [3, 4, 6, 8, 9, 10, 11]:
    assert not is_generalized_pentagonal(v), f"{v} should NOT be gen. pentagonal"

# Compute answer
N = 10 ** 7
gp = generalized_pentagonal(N)
answer = sum(gp)
print(f"Count: {len(gp)}")
print(answer)
