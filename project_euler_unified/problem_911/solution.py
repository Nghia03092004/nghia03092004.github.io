"""
Problem 911: Geometric Series Truncation

S(r, n) = sum_{k=0}^{n} r^k = (r^{n+1} - 1) / (r - 1).
Find sum_{r=2}^{100} S(r, r) mod 10^9+7.

Methods:
    1. Closed-form with modular inverse (Fermat)
    2. Direct polynomial evaluation (verification)
    3. Exact computation for small r (cross-check)
"""

from math import log10

MOD = 10**9 + 7

def solve_closed_form(mod: int):
    """Sum S(r, r) for r = 2..100, where S(r,r) = (r^{r+1}-1)/(r-1) mod p."""
    total = 0
    for r in range(2, 101):
        num = (pow(r, r + 1, mod) - 1) % mod
        inv = pow(r - 1, mod - 2, mod)
        total = (total + num * inv) % mod
    return total

def solve_direct(mod: int):
    """Compute each S(r,r) = 1 + r + r^2 + ... + r^r directly."""
    total = 0
    for r in range(2, 101):
        s = 0
        rk = 1  # r^k mod p
        for k in range(r + 1):
            s = (s + rk) % mod
            rk = rk * r % mod
        total = (total + s) % mod
    return total

def s_exact(r: int):
    """Compute S(r, r) = (r^{r+1} - 1) / (r - 1) exactly."""
    return (r ** (r + 1) - 1) // (r - 1)

# Solve
ans_closed = solve_closed_form(MOD)
ans_direct = solve_direct(MOD)

assert ans_closed == ans_direct, f"Mismatch: {ans_closed} vs {ans_direct}"

# Verify exact values for small r
assert s_exact(2) == 7      # 1 + 2 + 4
assert s_exact(3) == 40     # 1 + 3 + 9 + 27
assert s_exact(4) == 341    # 1 + 4 + 16 + 64 + 256
assert s_exact(5) == 3906
assert s_exact(10) == 11111111111

# Verify modular consistency
for r in range(2, 20):
    exact_mod = s_exact(r) % MOD
    num = (pow(r, r + 1, MOD) - 1) % MOD
    inv = pow(r - 1, MOD - 2, MOD)
    closed_mod = num * inv % MOD
    assert exact_mod == closed_mod, f"r={r}: exact={exact_mod}, closed={closed_mod}"

print(ans_closed)
