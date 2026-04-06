"""
Project Euler Problem 409: Nim Extreme
======================================
n piles with distinct sizes from {1, ..., 2^n - 1}. W(n) counts winning
positions (XOR != 0) for the first player, with piles ordered.

Formula:
  W(n) = n! * (2^n - 1) / 2^n * (C(2^n-1, n) - E_n)   mod M

where E_n = (-1)^h * C(2^{n-1}-1, h),  h = n//2   (n even)
      E_n = (-1)^{(n+1)/2} * C(2^{n-1}-1, (n-1)/2) (n odd)

All arithmetic mod M = 10^9 + 7.
"""

import time
import sys

M = 10**9 + 7

def solve(n):
    """Compute W(n) mod M in O(n) time."""
    pow2n = pow(2, n, M)
    pow2n1 = pow(2, n - 1, M)
    Nm1 = (pow2n - 1) % M          # 2^n - 1 mod M
    m_mod = (pow2n1 - 1) % M        # 2^{n-1} - 1 mod M

    # Compute n! mod M  and  C(2^n-1, n) mod M simultaneously
    fact_n = 1
    falling_Nm1 = 1
    for i in range(n):
        fact_n = fact_n * (i + 1) % M
        falling_Nm1 = falling_Nm1 * ((Nm1 - i) % M) % M
    inv_fact_n = pow(fact_n, M - 2, M)
    C_Nm1_n = falling_Nm1 * inv_fact_n % M

    # Compute C(m, h) mod M where m = 2^{n-1}-1, h = n//2
    h = n // 2
    fact_h = 1
    falling_m = 1
    for i in range(h):
        fact_h = fact_h * (i + 1) % M
        falling_m = falling_m * ((m_mod - i) % M) % M
    inv_fact_h = pow(fact_h, M - 2, M)
    C_m_h = falling_m * inv_fact_h % M

    # E_n with sign
    if n % 2 == 0:
        sign = 1 if h % 2 == 0 else -1
    else:
        sign = 1 if ((n + 1) // 2) % 2 == 0 else -1
    E_n_mod = sign * C_m_h % M

    # W(n) = n! * (2^n - 1) * inv(2^n) * (C(2^n-1,n) - E_n) mod M
    inv_pow2n = pow(pow2n, M - 2, M)
    W = fact_n * Nm1 % M * inv_pow2n % M * ((C_Nm1_n - E_n_mod) % M) % M
    return W

def brute_force(n):
    """Brute-force W(n) for small n (for verification)."""
    from itertools import combinations
    from math import factorial
    max_val = (1 << n) - 1
    winning = 0
    for combo in combinations(range(1, max_val + 1), n):
        xor_val = 0
        for v in combo:
            xor_val ^= v
        if xor_val != 0:
            winning += 1
    return winning * factorial(n)

def verify_small():
    """Verify against brute force and known values."""
    print("=== Verification against brute force ===")
    for n in range(1, 7):
        w_formula = solve(n)
        w_brute = brute_force(n) % M
        status = "OK" if w_formula == w_brute else "FAIL"
        print(f"  n={n}: formula={w_formula}, brute={w_brute}  [{status}]")

    print("\n=== Verification against problem examples ===")
    expected = {1: 1, 2: 6, 3: 168, 5: 19764360, 100: 384777056}
    for n, exp in expected.items():
        w = solve(n)
        status = "OK" if w == exp % M else "FAIL"
        print(f"  W({n}) mod M = {w}  (expected {exp % M})  [{status}]")

def make_visualization():
    """Generate a visualization and save to visualization.png."""
