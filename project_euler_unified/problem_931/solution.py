"""
Problem 931: Sum of Radical Expressions

For rad(d) = product of distinct prime factors of d, define
R(n) = sum_{d | n} rad(d). Compute sum_{n=1}^{N} R(n) mod (10^9 + 7).

Key insight:
  sum_{n=1}^{N} R(n) = sum_{n=1}^{N} sum_{d|n} rad(d)
                     = sum_{d=1}^{N} rad(d) * floor(N/d)
  by swapping summation order (each d divides floor(N/d) values of n).

Methods:
  - solve: Compute rad via modified sieve, then sum rad(d)*floor(N/d) mod p.
  - solve_small: Same logic for smaller N, also returns rad array for viz.
  - rad_brute: Brute-force rad(n) by trial division for verification.

Complexity: O(N log log N) for rad sieve, O(N) for the final sum.
"""

from collections import Counter

def solve(N=10**7, MOD=10**9 + 7):
    """Compute sum_{n=1}^{N} R(n) mod MOD via rad sieve."""
    rad = [1] * (N + 1)
    is_prime = [True] * (N + 1)
    for p in range(2, N + 1):
        if is_prime[p]:
            for m in range(p, N + 1, p):
                rad[m] *= p
                if m > p:
                    is_prime[m] = False
    S = 0
    for d in range(1, N + 1):
        S = (S + rad[d] * (N // d)) % MOD
    return S

def rad_brute(n):
    """Compute rad(n) by trial division."""
    if n == 1:
        return 1
    result = 1
    temp = n
    d = 2
    while d * d <= temp:
        if temp % d == 0:
            result *= d
            while temp % d == 0:
                temp //= d
        d += 1
    if temp > 1:
        result *= temp
    return result

def solve_small(N, MOD=10**9 + 7):
    """Solve for small N, return (answer, rad_array)."""
    rad = [1] * (N + 1)
    is_prime = [True] * (N + 1)
    for p in range(2, N + 1):
        if is_prime[p]:
            for m in range(p, N + 1, p):
                rad[m] *= p
                if m > p:
                    is_prime[m] = False
    S = 0
    for d in range(1, N + 1):
        S = (S + rad[d] * (N // d)) % MOD
    return S, rad

# Verification
# Check rad values
assert rad_brute(1) == 1
assert rad_brute(12) == 6  # 12 = 2^2 * 3 -> rad = 2*3 = 6
assert rad_brute(30) == 30  # 30 = 2*3*5 -> rad = 30
assert rad_brute(8) == 2   # 8 = 2^3 -> rad = 2

# Cross-check sieve rad vs brute force
_, rad_sieve = solve_small(100)
for n in range(1, 101):
    assert rad_sieve[n] == rad_brute(n), f"rad({n}): sieve={rad_sieve[n]}, brute={rad_brute(n)}"

# Verify small answer: sum R(n) for n=1..10
# R(1)=1, R(2)=3, R(3)=4, R(4)=5, R(5)=6,
# R(6)=12, R(7)=8, R(8)=7, R(9)=7, R(10)=18
# sum = 1+3+4+5+6+12+8+7+7+18 = 71
assert solve_small(10)[0] == 71, f"Expected 71, got {solve_small(10)[0]}"

# Compute answer
ans_small, rad_arr = solve_small(10**5)
print(f"Answer for N=10^5: {ans_small}")
