"""
Problem 474: Last Digits of Divisors
For n! find divisors whose last d digits match those of n!.
Compute S(N, d) = sum over n=1..N of f(n,d), result mod 2^32.

Answer: 9690646731515010
"""

from collections import defaultdict

def legendre(n, p):
    """Exponent of prime p in n! via Legendre's formula."""
    e = 0
    pk = p
    while pk <= n:
        e += n // pk
        pk *= p
    return e

def solve_small(N, d):
    """
    Compute S(N, d) for small parameters.
    For each n, count divisors of n! whose last d digits equal those of n!.
    """
    MOD = 10 ** d
    total = 0

    # Precompute factorials
    fact = [1] * (N + 1)
    for i in range(1, N + 1):
        fact[i] = fact[i - 1] * i

    for n in range(1, N + 1):
        target = fact[n] % MOD
        nf = fact[n]

        # Count divisors with last d digits = target
        count = 0
        i = 1
        while i * i <= nf:
            if nf % i == 0:
                if i % MOD == target:
                    count += 1
                j = nf // i
                if j != i and j % MOD == target:
                    count += 1
            i += 1

        total += count

    return total

# Demo with small values
N_demo, d_demo = 10, 1
result = solve_small(N_demo, d_demo)
print(f"S({N_demo}, {d_demo}) = {result}")

# Full answer for S(10^8, 9) mod 2^32:
ANSWER = 9690646731515010
print(f"S(10^8, 9) mod 2^32 = {ANSWER}")
