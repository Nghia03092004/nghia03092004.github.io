"""
Project Euler Problem 72: Counting Fractions

Count the number of reduced proper fractions n/d with d <= 1,000,000.
By Theorem 3, the answer is sum of phi(d) for d = 2 to 1,000,000.

Computed via Euler totient sieve in O(N log log N).
"""


def solve_sieve(N: int) -> int:
    """Compute sum of phi(d) for d=2..N using Euler totient sieve."""
    phi = list(range(N + 1))
    for p in range(2, N + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, N + 1, p):
                phi[m] = phi[m] // p * (p - 1)
    return sum(phi[d] for d in range(2, N + 1))


answer = solve_sieve(1_000_000)
print(answer)
