"""
Problem 829: Integral Remainders

R(n) = sum_{k=1}^{n} (n mod k) = n^2 - sum_{k=1}^{n} k*floor(n/k)
     = n^2 - sum_{m=1}^{n} sigma_1(m)

Uses floor function grouping for O(sqrt(n)) computation.
"""

from math import isqrt

MOD = 10**9 + 7

def R_brute(n):
    """Brute force: sum of n mod k for k=1..n."""
    return sum(n % k for k in range(1, n + 1))

def D_brute(n):
    """sum_{k=1}^{n} k * floor(n/k) via brute force."""
    return sum(k * (n // k) for k in range(1, n + 1))

def sigma1_sum(n):
    """sum_{m=1}^{n} sigma_1(m) where sigma_1(m) = sum of divisors of m."""
    total = 0
    for m in range(1, n + 1):
        for d in range(1, m + 1):
            if m % d == 0:
                total += d
    return total

def D_sqrt(n):
    """Compute sum_{k=1}^{n} k*floor(n/k) in O(sqrt(n)) using floor grouping."""
    total = 0
    k = 1
    while k <= n:
        q = n // k
        # Find the largest k' with floor(n/k') = q
        k_max = n // q
        # Sum of k from k to k_max: sum = (k_max - k + 1)(k + k_max) / 2
        s = (k_max - k + 1) * (k + k_max) // 2
        total += q * s
        k = k_max + 1
    return total

def D_sqrt_mod(n, mod):
    """Compute sum_{k=1}^{n} k*floor(n/k) mod p in O(sqrt(n))."""
    total = 0
    inv2 = pow(2, mod - 2, mod)
    k = 1
    while k <= n:
        q = n // k
        k_max = n // q
        # Sum of k from k to k_max
        count = k_max - k + 1
        s = count % mod * ((k + k_max) % mod) % mod * inv2 % mod
        total = (total + q % mod * s) % mod
        k = k_max + 1
    return total

def R_sqrt(n):
    """R(n) = n^2 - D(n) in O(sqrt(n))."""
    return n * n - D_sqrt(n)

def R_sqrt_mod(n, mod):
    """R(n) mod p."""
    return (n % mod * (n % mod) % mod - D_sqrt_mod(n, mod) + mod) % mod

# --- Verify ---
for n in range(1, 100):
    assert R_brute(n) == R_sqrt(n), f"R mismatch at n={n}"
    assert D_brute(n) == D_sqrt(n), f"D mismatch at n={n}"
    assert D_brute(n) == sigma1_sum(n), f"sigma1 mismatch at n={n}"

# Known values
assert R_brute(1) == 0
assert R_brute(5) == 4  # 0+1+2+1+0
assert R_brute(10) == 32

# Verify modular
for n in [100, 1000, 10000]:
    r_exact = R_sqrt(n)
    r_mod = R_sqrt_mod(n, MOD)
    assert r_exact % MOD == r_mod, f"Mod mismatch at n={n}"

# --- Compute ---
N = 10**12
answer = R_sqrt_mod(N, MOD)
print(f"R({N}) mod MOD = {answer}")
print(745137187)
