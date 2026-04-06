"""
Problem 847: Jack's Bean

h(a,b,c) = ceil(log2(a+b+c)) = minimum questions to find magic bean among a+b+c beans.
H(N) = sum of h(a,b,c) over all triples with 1 <= a+b+c <= N.

H(N) = sum_{n=1}^{N} ceil(log2(n)) * C(n+2, 2)

We group by k = ceil(log2(n)) and use hockey-stick identity.
"""

import math

MOD = 1_000_000_007

def modinv(a, m=MOD):
    return pow(a, m - 2, m)

def C3(n):
    """Compute C(n,3) mod MOD = n*(n-1)*(n-2)/6"""
    if n < 3:
        return 0
    return (n % MOD) * ((n-1) % MOD) % MOD * ((n-2) % MOD) % MOD * modinv(6) % MOD

def sum_C_n2_2(a, b):
    """Sum of C(n+2,2) for n = a..b = C(b+3,3) - C(a+2,3)"""
    return (C3(b + 3) - C3(a + 2)) % MOD

def H(N):
    if N <= 0:
        return 0
    result = 0
    # n=1: ceil(log2(1)) = 0, contributes 0
    # For k >= 1: n in [2^(k-1)+1, 2^k]
    k = 1
    while True:
        lo = (1 << (k - 1)) + 1
        if k == 1:
            lo = 2  # ceil(log2(2)) = 1
        hi = min(1 << k, N)
        if lo > N:
            break
        # n=1 has ceil(log2(1))=0, so skip
        # Actually ceil(log2(1)) = 0
        # ceil(log2(2)) = 1
        # ceil(log2(3)) = 2, ceil(log2(4)) = 2
        # ceil(log2(5)) = 3, ..., ceil(log2(8)) = 3
        # In general, ceil(log2(n)) = k iff 2^(k-1) < n <= 2^k
        # Special: ceil(log2(1)) = 0, ceil(log2(2)) = 1
        s = sum_C_n2_2(lo, hi)
        result = (result + k * s) % MOD
        if hi == N:
            break
        k += 1
    return result % MOD

# Verify with test cases
def H_brute(N):
    total = 0
    for n in range(1, N + 1):
        k = math.ceil(math.log2(n)) if n > 1 else 0
        total += k * (n + 1) * (n + 2) // 2
    return total

print(f"H(6) brute = {H_brute(6)}")
print(f"H(20) brute = {H_brute(20)}")
print(f"H(111) brute = {H_brute(111)}")

print(f"H(6) = {H(6)}")
print(f"H(20) = {H(20)}")
print(f"H(111) = {H(111)}")

R19 = int("1" * 19)
print(f"H(R_19) mod 10^9+7 = {H(R19)}")
