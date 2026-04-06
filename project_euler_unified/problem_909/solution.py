"""
Problem 909: Random Walk Return Probability

Symmetric random walk on Z starting at 0. R(2n) = probability of being
at 0 after 2n steps. Find R(20) = p/q in lowest terms, compute p + q.

Key formula: R(2n) = C(2n, n) / 4^n
Asymptotic:  R(2n) ~ 1 / sqrt(pi * n)

Methods:
    1. Direct computation with gcd reduction
    2. Exact combinatorial verification
    3. Monte Carlo simulation (verification)
"""

from math import comb, gcd, sqrt, pi
import random

def return_prob_exact(n: int):
    """Compute R(2n) = C(2n,n) / 4^n as reduced fraction (p, q)."""
    num = comb(2 * n, n)
    den = 4 ** n
    g = gcd(num, den)
    return num // g, den // g

def solve(n: int = 10):
    """Return p + q where R(2n) = p/q reduced."""
    p, q = return_prob_exact(n)
    return p + q

def return_prob_brute(n: int) -> float:
    """Count paths of 2n steps returning to 0, brute-force for small n."""
    count = 0
    total = 0
    # Enumerate all 2^(2n) paths (only feasible for small n)
    for mask in range(1 << (2 * n)):
        pos = 0
        for step in range(2 * n):
            if mask & (1 << step):
                pos += 1
            else:
                pos -= 1
        if pos == 0:
            count += 1
        total += 1
    return count / total

def return_prob_monte_carlo(n: int, trials: int = 500000) -> float:
    """Estimate R(2n) by simulation."""
    random.seed(42)
    returns = 0
    for _ in range(trials):
        pos = 0
        for _ in range(2 * n):
            pos += random.choice([-1, 1])
        if pos == 0:
            returns += 1
    return returns / trials

# Solve
ans = solve(10)

# Verify known values
assert return_prob_exact(1) == (1, 2)   # R(2) = 1/2
assert return_prob_exact(2) == (3, 8)   # R(4) = 3/8
assert return_prob_exact(3) == (5, 16)  # R(6) = 5/16

# Verify with brute force for small n
for test_n in [1, 2, 3, 4]:
    p, q = return_prob_exact(test_n)
    brute = return_prob_brute(test_n)
    assert abs(p / q - brute) < 1e-12, f"n={test_n}: exact={p/q}, brute={brute}"

# Verify specific answer
p, q = return_prob_exact(10)
assert p == 46189 and q == 262144
assert ans == 308333

print(ans)
