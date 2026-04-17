"""
Problem 839: Beans in Bowls

Count distributions of n beans into k bowls, each with capacity m.
B(n,k,m) = sum_{j=0}^{floor(n/(m+1))} (-1)^j * C(k,j) * C(n-j(m+1)+k-1, k-1)
"""

from math import comb
from functools import lru_cache

MOD = 10**9 + 7

# --- Method 1: Inclusion-Exclusion ---
def beans_inclusion_exclusion(n: int, k: int, m: int):
    """Inclusion-exclusion formula for bounded compositions."""
    result = 0
    for j in range(k + 1):
        rem = n - j * (m + 1)
        if rem < 0:
            break
        term = comb(k, j) * comb(rem + k - 1, k - 1)
        if j % 2 == 0:
            result += term
        else:
            result -= term
    return result

# --- Method 2: Dynamic Programming ---
def beans_dp(n: int, k: int, m: int):
    """DP approach with prefix-sum optimization."""
    # dp[s] = number of ways to fill first i bowls to sum s
    dp = [0] * (n + 1)
    dp[0] = 1
    for _ in range(k):
        new_dp = [0] * (n + 1)
        prefix = [0] * (n + 2)
        for s in range(n + 1):
            prefix[s + 1] = prefix[s] + dp[s]
        for s in range(n + 1):
            lo = max(0, s - m)
            new_dp[s] = prefix[s + 1] - prefix[lo]
        dp = new_dp
    return dp[n]

# --- Method 3: Generating function (polynomial multiplication) ---
def beans_genfunc(n: int, k: int, m: int):
    """Multiply the polynomial (1 + x + ... + x^m) k times."""
    poly = [1] * (m + 1)  # single bowl GF
    result = [1]  # identity polynomial
    for _ in range(k):
        new_result = [0] * (len(result) + len(poly) - 1)
        for i, a in enumerate(result):
            for j, b in enumerate(poly):
                if i + j <= n:
                    new_result[i + j] += a * b
        result = new_result[:n + 1]
    return result[n] if n < len(result) else 0

# --- Verify all methods agree ---
for nn in range(0, 20):
    for kk in range(1, 6):
        for mm in range(1, 8):
            a = beans_inclusion_exclusion(nn, kk, mm)
            b = beans_dp(nn, kk, mm)
            c = beans_genfunc(nn, kk, mm)
            assert a == b == c, f"Mismatch at ({nn},{kk},{mm}): IE={a}, DP={b}, GF={c}"

print("All verification passed!")

# Modular version for large inputs
def modinv(a, m=MOD):
    return pow(a, m - 2, m)

def comb_mod(n, r, mod=MOD):
    if r < 0 or r > n:
        return 0
    if r == 0:
        return 1
    num = 1
    den = 1
    for i in range(r):
        num = num * ((n - i) % mod) % mod
        den = den * ((i + 1) % mod) % mod
    return num * modinv(den, mod) % mod

def beans_mod(n, k, m, mod=MOD):
    result = 0
    for j in range(k + 1):
        rem = n - j * (m + 1)
        if rem < 0:
            break
        term = comb_mod(k, j, mod) * comb_mod(rem + k - 1, k - 1, mod) % mod
        if j % 2 == 0:
            result = (result + term) % mod
        else:
            result = (result - term + mod) % mod
    return result

# Example computation
answer = beans_mod(1000, 100, 20)
print(answer)
