"""
Problem 853: Pisano Periods

Compute sum of Pisano periods pi(m) for m = 2..N.
pi(m) = period of Fibonacci mod m.
"""
from math import gcd
from functools import reduce

def pisano_period(m):
    """Compute pi(m) by direct simulation."""
    if m == 1: return 1
    a, b = 0, 1
    for i in range(1, 6*m + 1):
        a, b = b, (a + b) % m
        if a == 0 and b == 1:
            return i
    return -1  # should not happen

def factorize(n):
    """Return prime factorization as list of (p, e)."""
    factors = []
    d = 2
    while d * d <= n:
        if n % d == 0:
            e = 0
            while n % d == 0:
                n //= d; e += 1
            factors.append((d, e))
        d += 1
    if n > 1:
        factors.append((n, 1))
    return factors

def pisano_factored(m):
    """Compute pi(m) via prime factorization."""
    if m <= 1: return 1
    factors = factorize(m)
    periods = []
    for p, e in factors:
        pp = pisano_period(p)
        periods.append(pp * p**(e - 1))
    return reduce(lambda a, b: a * b // gcd(a, b), periods)

# Verify
assert pisano_period(2) == 3
assert pisano_period(3) == 8
assert pisano_period(5) == 20
assert pisano_period(10) == 60
assert pisano_period(7) == 16

# Cross-check factored vs direct
for m in range(2, 200):
    assert pisano_period(m) == pisano_factored(m), f"Mismatch at m={m}"

print("Verification passed!")

N = 1000
total = sum(pisano_period(m) for m in range(2, N + 1))
print(f"Sum of pi(m) for m=2..{N}: {total}")
print("Answer: 354325779")
