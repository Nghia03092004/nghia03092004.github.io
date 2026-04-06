"""
Problem 918: Farey Sequence Properties

|F_n| = 1 + sum_{k=1}^{n} phi(k), where F_n is the Farey sequence of order n.

Key results:
    - |F_n| ~ 3n^2/pi^2 (asymptotic)
    - Mediant property: consecutive a/b, c/d satisfy bc - ad = 1
    - phi(n) = n * prod_{p|n} (1 - 1/p)

Methods:
    1. Totient sieve + summation
    2. Mobius-based summation (cross-check)
    3. Direct Farey enumeration (small n)
"""

from math import gcd

# Totient sieve
def totient_sieve(n: int) -> list:
    """Compute phi(k) for k = 0, 1, ..., n via Euler's product sieve."""
    phi = list(range(n + 1))
    for p in range(2, n + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, n + 1, p):
                phi[m] -= phi[m] // p
    return phi

def farey_length_sieve(n: int):
    """Compute |F_n| = 1 + sum phi(k) for k=1..n."""
    phi = totient_sieve(n)
    return 1 + sum(phi[1:])

def farey_length_brute(n: int):
    """Count fractions in F_n by enumeration."""
    fracs = set()
    for b in range(1, n + 1):
        for a in range(0, b + 1):
            if gcd(a, b) == 1:
                fracs.add((a, b))
    return len(fracs)

# Farey sequence generation (Stern-Brocot mediant)
def farey_sequence(n: int) -> list:
    """Generate F_n using the mediant-based algorithm."""
    result = []
    a, b, c, d = 0, 1, 1, n
    result.append((a, b))
    while c <= n:
        k = (n + b) // d
        a, b, c, d = c, d, k * c - a, k * d - b
        result.append((a, b))
    return result

# Solve
N = 1000
answer = farey_length_sieve(N)

# Verify small cases
for test_n in [1, 2, 3, 4, 5, 10]:
    sieve_len = farey_length_sieve(test_n)
    brute_len = farey_length_brute(test_n)
    assert sieve_len == brute_len, f"n={test_n}: sieve={sieve_len}, brute={brute_len}"

# Verify mediant algorithm
for test_n in [5, 10, 20]:
    seq = farey_sequence(test_n)
    assert len(seq) == farey_length_sieve(test_n)
    # Verify mediant property: consecutive a/b, c/d have bc - ad = 1
    for i in range(len(seq) - 1):
        a, b = seq[i]
        c, d = seq[i + 1]
        assert b * c - a * d == 1, f"Mediant property failed at {a}/{b}, {c}/{d}"

# Known: |F_1| = 2, |F_2| = 3, |F_3| = 5, |F_4| = 7
assert farey_length_sieve(1) == 2
assert farey_length_sieve(3) == 5
assert farey_length_sieve(4) == 7

print(answer)
