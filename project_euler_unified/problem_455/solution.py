#!/usr/bin/env python3
"""
Project Euler Problem 455: Powers With Trailing Digits

f(n) = largest x in [1, 10^9) such that n^x = x (mod 10^9), or 0 if none.
Find sum f(n) for 2 <= n <= 10^6.

Algorithm: CRT decomposition into mod 2^9 and mod 5^9.
For each prime power p^k, solve n^x = x (mod p^k) using Hensel-style lifting
over the combined modulus M_j = lcm(p^j, lambda(p^j)), which ensures both
the residue and exponent are correctly tracked.
Then combine solutions via CRT and verify.

Checkpoints: f(4) = 411728896, f(157) = 743757, f(10) = 0.
sum f(n) for 2 <= n <= 10^3 = 442530011399.
Answer: sum f(n) for 2 <= n <= 10^6 = 450186511399999.
"""

import math
import time

MOD = 10**9
PK2 = 2**9   # 512
PK5 = 5**9   # 1953125

def my_lcm(a, b):
    return a * b // math.gcd(a, b)

def carmichael(p, j):
    """Carmichael function lambda(p^j)."""
    if j == 0:
        return 1
    if p == 2:
        if j == 1:
            return 1
        if j == 2:
            return 2
        return 2**(j - 2)
    return (p - 1) * p**(j - 1)

def combined_mod(p, j):
    """M_j = lcm(p^j, lambda(p^j)): modulus tracking both residue and exponent."""
    if j == 0:
        return 1
    return my_lcm(p**j, carmichael(p, j))

def solve_mod_pk(n, p, k):
    """
    Find all x mod M_k such that n^x = x (mod p^k),
    where M_k = lcm(p^k, lambda(p^k)).

    If p | n, then n^x = 0 (mod p^k) for large x, so x = 0 (mod p^k).
    Returns list of residues mod p^k (the [0] case for p|n, or derived from lifting).
    """
    if n % p == 0:
        # n^x = 0 mod p^k for x >= ceil(k / v_p(n)).
        # Need x = 0 mod p^k. Return [0] as the residue.
        return [0]

    # Base case: level 1
    M1 = combined_mod(p, 1)
    solutions = []
    for x in range(M1):
        if pow(n, x, p) == x % p:
            solutions.append(x)

    # Lift from level j to j+1
    for j in range(1, k):
        Mj = combined_mod(p, j)
        Mj1 = combined_mod(p, j + 1)
        branch = Mj1 // Mj
        pj1 = p ** (j + 1)
        new_solutions = []
        for xj in solutions:
            for t in range(branch):
                x_cand = xj + t * Mj
                if pow(n, x_cand, pj1) == x_cand % pj1:
                    new_solutions.append(x_cand)
        solutions = new_solutions
        if not solutions:
            return []

    return solutions

def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x, y = extended_gcd(b, a % b)
    return g, y, x - (a // b) * y

def compute_f(n):
    """
    Compute f(n): largest x in [1, 10^9) with n^x = x (mod 10^9).

    1. Solve n^x = x (mod 2^9) via lifting.
    2. Solve n^x = x (mod 5^9) via lifting.
    3. CRT combine, verify, take maximum.
    """
    if n % 10 == 0:
        return 0

    sols2_raw = solve_mod_pk(n, 2, 9)
    if not sols2_raw:
        return 0

    sols5_raw = solve_mod_pk(n, 5, 9)
    if not sols5_raw:
        return 0

    # Extract distinct residues mod p^k
    sols2 = list(set(s % PK2 for s in sols2_raw))
    sols5 = list(set(s % PK5 for s in sols5_raw))

    best = 0
    for r2 in sols2:
        for r5 in sols5:
            g, u, v = extended_gcd(PK2, PK5)
            if (r5 - r2) % g != 0:
                continue
            lcm_val = PK2 * PK5 // g
            x = (r2 + PK2 * ((r5 - r2) // g) * u) % lcm_val
            if x <= 0 or x >= MOD:
                continue
            # Verify (CRT gives necessary but not always sufficient conditions)
            if pow(n, x, MOD) == x:
                best = max(best, x)

    return best

def solve(N_max):
    """Compute sum of f(n) for 2 <= n <= N_max."""
    total = 0
    count_nonzero = 0
    t0 = time.time()

    for n in range(2, N_max + 1):
        fn = compute_f(n)
        total += fn
        if fn > 0:
            count_nonzero += 1
        if n % 100000 == 0:
            elapsed = time.time() - t0
            print(f"  n = {n}, sum = {total}, nonzero = {count_nonzero}, "
                  f"time = {elapsed:.1f}s")

    return total, count_nonzero

def create_visualization(save_path, sample_size=2000):
    """Create visualization of f(n) distribution."""
    print("  Computing f(n) for visualization sample...")
    ns = list(range(2, sample_size + 2))
    f_values = [compute_f(n) for n in ns]
