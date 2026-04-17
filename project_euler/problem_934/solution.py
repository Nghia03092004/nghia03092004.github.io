"""
Problem 934: Primitive Root Sequences

For each prime p, let g(p) be the smallest primitive root modulo p.
Compute S(N) = sum of g(p) for all primes p <= N.

Key results:
  - A primitive root g mod p has multiplicative order p-1.
  - g(2) = 1 (by convention).
  - The smallest primitive root is often 2 or 3; g(p) = 2 is most common.
  - Artin's conjecture: 2 is a primitive root for infinitely many primes.

Methods:
  - sieve_primes: Sieve of Eratosthenes.
  - factorize: Trial division to find prime factors of p-1.
  - smallest_prim_root: Test candidates g=2,3,... using the standard criterion.
  - solve: Sum g(p) for all primes p <= N.

Complexity: O(N log log N) for sieve + O(p^{1/4} * log p) per prime on average.
"""

from collections import Counter

# Sieve of Eratosthenes
def sieve_primes(n):
    """Return list of primes up to n."""
    is_p = [True] * (n + 1)
    is_p[0] = is_p[1] = False
    for i in range(2, int(n**0.5) + 1):
        if is_p[i]:
            for j in range(i * i, n + 1, i):
                is_p[j] = False
    return [i for i in range(2, n + 1) if is_p[i]]

# Factorize via trial division
def factorize(n):
    """Return set of distinct prime factors of n."""
    factors = set()
    d = 2
    while d * d <= n:
        while n % d == 0:
            factors.add(d)
            n //= d
        d += 1
    if n > 1:
        factors.add(n)
    return factors

# Smallest primitive root
def smallest_prim_root(p):
    """Find the smallest primitive root modulo prime p."""
    if p == 2:
        return 1
    phi = p - 1
    factors = factorize(phi)
    for g in range(2, p):
        if all(pow(g, phi // q, p) != 1 for q in factors):
            return g
    return -1

# Method: Check if g is a primitive root
def is_primitive_root(g, p):
    """Check whether g is a primitive root mod p."""
    if p == 2:
        return g % 2 == 1
    phi = p - 1
    factors = factorize(phi)
    return all(pow(g, phi // q, p) != 1 for q in factors)

# Solve: sum of smallest primitive roots
def solve(N):
    """Compute S(N) = sum of g(p) for primes p <= N."""
    primes = sieve_primes(N)
    return sum(smallest_prim_root(p) for p in primes)

# Verification
# Known smallest primitive roots:
assert smallest_prim_root(2) == 1
assert smallest_prim_root(3) == 2
assert smallest_prim_root(5) == 2
assert smallest_prim_root(7) == 3
assert smallest_prim_root(11) == 2
assert smallest_prim_root(13) == 2
assert smallest_prim_root(17) == 3
assert smallest_prim_root(23) == 5
assert smallest_prim_root(41) == 6

# Verify is_primitive_root
assert is_primitive_root(3, 7)
assert not is_primitive_root(2, 7)  # 2^3 = 8 = 1 mod 7, order 3 != 6

# Compute answer
primes = sieve_primes(10000)
roots = [smallest_prim_root(p) for p in primes]
S = sum(roots)
print(f"S(10^4) = {S}")
