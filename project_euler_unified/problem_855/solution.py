"""
Problem 855: Power Sequence - Modular Tetration

Compute {}^n a mod m using iterated Euler's theorem.
"""
from math import gcd

def euler_totient(n):
    result = n
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0: temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def tetration_mod(a, n, m):
    """Compute {}^n a mod m."""
    if m == 1: return 0
    if n == 0: return 1 % m
    if n == 1: return a % m
    phi_m = euler_totient(m)
    if gcd(a, m) == 1:
        exp = tetration_mod(a, n - 1, phi_m)
        return pow(a, exp, m)
    else:
        exp = tetration_mod(a, n - 1, phi_m)
        # Need to handle case where a and m share factors
        # For large enough towers, a^tower is divisible by high powers of shared primes
        return pow(a, exp + phi_m, m)  # add phi_m to ensure correct residue

# Direct computation for small cases
def tetration_exact(a, n):
    if n == 0: return 1
    return a ** tetration_exact(a, n - 1)

# Verify
assert tetration_mod(2, 1, 10) == 2
assert tetration_mod(2, 2, 10) == 4
assert tetration_mod(2, 3, 10) == 6
assert tetration_mod(2, 4, 10) == 6
assert tetration_mod(3, 2, 7) == 6

for a in range(2, 6):
    for n in range(0, 5):
        exact = tetration_exact(a, n)
        for m in [7, 10, 13, 100]:
            assert tetration_mod(a, n, m) == exact % m, \
                f"Mismatch: a={a}, n={n}, m={m}"

print("Verification passed!")
print(f"Answer: {tetration_mod(2, 100, 10**9 + 7)}")
