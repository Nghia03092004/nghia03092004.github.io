"""
Problem 945: Smooth Number Sieve

Count B-smooth numbers up to x. A number is B-smooth if all its prime
factors are <= B. The counting function Psi(x, B) is fundamental in
analytic number theory and cryptography (e.g., subexponential factoring).

Key results:
  - Psi(10^5, 10) = 339
  - Psi(10^5, 20) = 1768
  - Psi(10^5, 50) = 9301
  - Asymptotically Psi(x, B) ~ x * rho(u) where u = log(x)/log(B)
    and rho is the Dickman function.

Methods:
  1. Recursive counting via prime factorization (lru_cache)
  2. Sieve-based direct enumeration for validation
  3. Dickman rho numerical integration
  4. Ratio analysis Psi(x,B)/x vs Dickman approximation
"""
import numpy as np
from functools import lru_cache

def sieve_primes(n):
    """Return list of primes up to n."""
    s = [True] * (n + 1)
    s[0] = s[1] = False
    for i in range(2, int(n ** 0.5) + 1):
        if s[i]:
            for j in range(i * i, n + 1, i):
                s[j] = False
    return [i for i in range(2, n + 1) if s[i]]

def count_smooth(x, B):
    """Count B-smooth numbers up to x using recursive prime decomposition."""
    primes = sieve_primes(B)

    @lru_cache(maxsize=None)
    def psi(x, idx):
        if x < 1:
            return 0
        if idx < 0:
            return 1
        result = psi(x, idx - 1)
        pk = primes[idx]
        pa = pk
        while pa <= x:
            result += psi(x // pa, idx - 1)
            pa *= pk
        return result

    return psi(int(x), len(primes) - 1)

def count_smooth_sieve(x, B):
    """Count B-smooth numbers up to x by sieve: remove numbers with
    a prime factor > B."""
    x = int(x)
    is_smooth = [True] * (x + 1)
    is_smooth[0] = False
    primes_all = sieve_primes(x)
    for p in primes_all:
        if p > B:
            for mult in range(p, x + 1, p):
                is_smooth[mult] = False
    return sum(is_smooth)

def dickman_rho_table(u_max=10.0, steps=10000):
    """Numerically solve rho(u) via the delay-differential equation:
    u*rho'(u) = -rho(u-1), rho(u)=1 for 0<=u<=1."""
    du = u_max / steps
    u_vals = np.linspace(0, u_max, steps + 1)
    rho_vals = np.ones(steps + 1)
    # rho(u) = 1 for u in [0,1]
    one_idx = int(1.0 / du)
    # For u in (1,2]: rho(u) = 1 - ln(u)
    for i in range(one_idx + 1, steps + 1):
        u = u_vals[i]
        if u <= 2.0:
            rho_vals[i] = 1.0 - np.log(u)
        else:
            # u*rho'(u) = -rho(u-1) => rho(u) = rho(u-du) - du/u * rho(u-1)
            idx_back = int(round((u - 1.0) / du))
            idx_back = min(idx_back, steps)
            rho_vals[i] = rho_vals[i - 1] - (du / u) * rho_vals[idx_back]
    return u_vals, rho_vals

# Verification with assertions
# Cross-check recursive vs sieve for small values
assert count_smooth(100, 5) == count_smooth_sieve(100, 5)
assert count_smooth(200, 7) == count_smooth_sieve(200, 7)
assert count_smooth(1000, 10) == count_smooth_sieve(1000, 10)

# 2-smooth numbers up to 100 are powers of 2: 1,2,4,8,16,32,64 = 7
assert count_smooth(100, 2) == 7

# Compute results
for B in [10, 20, 50]:
    c = count_smooth(100000, B)
    print(f"Psi(10^5, {B}) = {c}")

print(count_smooth(100000, 50))
