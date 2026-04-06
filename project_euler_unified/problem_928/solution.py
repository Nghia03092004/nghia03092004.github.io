"""
Problem 928: Catalan Number Variants

Compute the sum of Catalan numbers C_0 + C_1 + ... + C_N modulo 10^9+7.
Uses modular arithmetic with precomputed factorials and inverse factorials.

Key ideas:
    - Catalan number: C_n = C(2n, n) / (n+1) = (2n)! / (n! * (n+1)!).
    - Recurrence: C_{n+1} = C_n * 2(2n+1) / (n+2).
    - Asymptotic: C_n ~ 4^n / (n^{3/2} * sqrt(pi)).
    - Generating function: C(x) = (1 - sqrt(1-4x)) / (2x).

Methods:
    1. Modular arithmetic with factorial precomputation
    2. Recurrence relation (exact integers for small n)
    3. Asymptotic approximation comparison
    4. Ratio analysis C_{n+1}/C_n approaching 4
"""

import numpy as np

def solve(N=1000):
    """Compute sum of C_0..C_N mod 10^9+7."""
    MOD = 10**9 + 7
    fact = [1] * (2 * N + 2)
    for i in range(1, 2 * N + 2):
        fact[i] = fact[i - 1] * i % MOD
    inv_fact = [1] * (2 * N + 2)
    inv_fact[2 * N + 1] = pow(fact[2 * N + 1], MOD - 2, MOD)
    for i in range(2 * N, -1, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD

    def catalan_mod(k):
        return fact[2 * k] * inv_fact[k] % MOD * inv_fact[k + 1] % MOD

    total = 0
    for k in range(N + 1):
        total = (total + catalan_mod(k)) % MOD
    return total

def catalan_exact(N):
    """Compute exact Catalan numbers C_0..C_N using the recurrence."""
    cats = [1]
    for k in range(1, N + 1):
        cats.append(cats[-1] * 2 * (2 * k - 1) // (k + 1))
    return cats

def catalan_asymptotic(n):
    """Asymptotic estimate: C_n ~ 4^n / (n^{3/2} * sqrt(pi))."""
    if n == 0:
        return 1.0
    return 4**n / (n**1.5 * np.sqrt(np.pi))

def catalan_ratios(cats):
    """Compute C_{n+1}/C_n for each n. Approaches 4."""
    return [cats[i + 1] / cats[i] for i in range(len(cats) - 1)]

# Solve and verify
answer = solve()

# Verify exact Catalan numbers against known values
cats = catalan_exact(20)
known = [1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796]
for i, val in enumerate(known):
    assert cats[i] == val, f"Catalan({i}) mismatch: {cats[i]} != {val}"

# Verify formula: C_n = C(2n,n)/(n+1)
from math import comb
for n in range(15):
    assert cats[n] == comb(2 * n, n) // (n + 1), f"Binomial formula mismatch at {n}"

# Verify sum of first few
assert sum(cats[:5]) == 1 + 1 + 2 + 5 + 14  # = 23

print(answer)
