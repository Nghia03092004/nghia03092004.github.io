"""
Problem 926: Binomial Coefficient Divisibility

Count non-zero entries mod p in Pascal's triangle rows 0..N. For each row n,
the number of entries C(n,k) not divisible by p equals the product of (d_i + 1)
where d_i are the base-p digits of n (Kummer's theorem / Lucas' theorem).

Key ideas:
    - By Lucas' theorem, C(n,k) != 0 mod p iff each base-p digit of k <= digit of n.
    - Number of non-zero entries in row n = product of (d_i + 1) for base-p digits d_i.
    - For p=2, this equals 2^(popcount(n)), giving a self-similar fractal structure.
    - Sierpinski triangle pattern emerges in Pascal's triangle mod 2.

Methods:
    1. Digit-product formula (Lucas' theorem application)
    2. Direct binomial computation for small cases (verification)
    3. Fractal dimension analysis
    4. Comparison across different primes
"""

import numpy as np
from math import comb

def count_nonzero_row(n, p):
    """Count entries in row n of Pascal's triangle not divisible by p."""
    prod = 1
    while n > 0:
        prod *= (n % p + 1)
        n //= p
    return prod

def solve(N=1000, p=2):
    """Sum of non-zero-mod-p entries across rows 0..N."""
    total = 0
    for n in range(N + 1):
        total += count_nonzero_row(n, p)
    return total

def count_nonzero_row_brute(n, p):
    """Count entries in row n not divisible by p by direct computation."""
    count = 0
    for k in range(n + 1):
        if comb(n, k) % p != 0:
            count += 1
    return count

def fractal_dimension_estimate(p, max_power=10):
    """Estimate fractal dimension: total non-zero entries up to p^k rows.

    For Pascal mod p, the fractal dimension is log(p*(p+1)/2) / log(p).
    """
    ratios = []
    for k in range(2, max_power + 1):
        N = p**k
        total = sum(count_nonzero_row(n, p) for n in range(N))
        if k > 2:
            prev_N = p**(k - 1)
            prev_total = sum(count_nonzero_row(n, p) for n in range(prev_N))
            ratios.append(np.log(total / prev_total) / np.log(p))
    return ratios

def row_counts_for_prime(N, p):
    """Return list of non-zero entry counts per row for given prime."""
    return [count_nonzero_row(n, p) for n in range(N)]

# Solve and verify
answer = solve()

# Verify digit-product formula against brute force for small cases
for n in range(50):
    assert count_nonzero_row(n, 2) == count_nonzero_row_brute(n, 2), f"Mismatch at row {n}"

# Verify for p=3
for n in range(40):
    assert count_nonzero_row(n, 3) == count_nonzero_row_brute(n, 3), f"Mismatch at row {n} for p=3"

# Known: row 2^k - 1 has all entries odd (all 2^k entries are odd)
for k in range(1, 8):
    n = 2**k - 1
    assert count_nonzero_row(n, 2) == 2**k

print(answer)
