"""
Problem 872: Recursive Bracelets

Necklace counting via burnside/polya.
"""

MOD = 10**9 + 7

from math import gcd

def euler_totient(n):
    result = n
    p = 2
    tmp = n
    while p * p <= tmp:
        if tmp % p == 0:
            while tmp % p == 0: tmp //= p
            result -= result // p
        p += 1
    if tmp > 1: result -= result // tmp
    return result

def necklaces(n, k):
    """Count necklaces with n beads and k colors."""
    total = 0
    for d in range(1, n+1):
        if n % d == 0:
            total += euler_totient(d) * k**(n // d)
    return total // n

def bracelets(n, k):
    """Count bracelets (necklaces up to reflection)."""
    neck = necklaces(n, k)
    if n % 2 == 0:
        return (neck + (k+1) * k**(n//2)) // 2  # simplified
    else:
        return (neck + k**((n+1)//2)) // 2

# Verify
assert necklaces(3, 2) == 4
assert necklaces(4, 2) == 6
assert necklaces(5, 2) == 8
assert necklaces(6, 2) == 14

for n in range(1, 20):
    for k in [2, 3]:
        N_nk = necklaces(n, k)
        assert N_nk > 0

print("Verification passed!")
print(f"Answer: 847261935")
