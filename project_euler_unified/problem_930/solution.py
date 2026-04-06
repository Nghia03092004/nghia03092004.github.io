"""
Problem 930: Quadratic Residue Patterns

For a prime p, define R(p) = sum_{a=1}^{p-1} L(a,p) * L(a+1,p) where L is
the Legendre symbol. Compute sum of R(p) for all primes p <= N = 10^5.

Key results:
  - R(2) = 0.
  - R(p) = -1 for all odd primes p (classical result from character sums).
  - Therefore sum = 0 + (-1) * (number_of_odd_primes_up_to_N).

Methods:
  - solve: O(N log log N) sieve, then apply the formula directly.
  - R_brute: O(p^2) brute-force computation of R(p) for verification.
  - legendre: Euler criterion for the Legendre symbol.

Complexity: O(N log log N) for sieve; verification is O(p) per prime.
"""

from collections import Counter

# Core functions
def legendre(a, p):
    """Compute the Legendre symbol (a/p) via Euler's criterion."""
    if a % p == 0:
        return 0
    return 1 if pow(a, (p - 1) // 2, p) == 1 else -1

def R_brute(p):
    """Brute-force computation of R(p) = sum L(a,p)*L(a+1,p)."""
    return sum(legendre(a, p) * legendre((a + 1) % p, p) for a in range(1, p))

def solve(N=10**5):
    """Sum of R(p) for primes p <= N. Uses R(2)=0, R(odd p)=-1."""
    sieve = [True] * (N + 1)
    sieve[0] = sieve[1] = False
    for i in range(2, int(N**0.5) + 1):
        if sieve[i]:
            for j in range(i * i, N + 1, i):
                sieve[j] = False
    primes = [p for p in range(2, N + 1) if sieve[p]]
    # R(2) = 0, R(p) = -1 for all odd primes
    total = 0 + (-1) * (len(primes) - 1)
    return total

def qr_set(p):
    """Return the set of quadratic residues mod p."""
    return set(x * x % p for x in range(1, p))

# Verification
assert R_brute(3) == -1
assert R_brute(5) == -1
assert R_brute(7) == -1
assert R_brute(11) == -1
assert R_brute(13) == -1
assert R_brute(2) == 0

# Cross-check solve against brute force for small N
for test_N in [10, 50, 100]:
    sieve_t = [True] * (test_N + 1)
    sieve_t[0] = sieve_t[1] = False
    for i in range(2, int(test_N**0.5) + 1):
        if sieve_t[i]:
            for j in range(i * i, test_N + 1, i):
                sieve_t[j] = False
    ps = [p for p in range(2, test_N + 1) if sieve_t[p]]
    brute_sum = sum(R_brute(p) for p in ps)
    assert solve(test_N) == brute_sum, f"Mismatch at N={test_N}"

# Compute answer
answer = solve()
print(answer)
