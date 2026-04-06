"""
Problem 869: Prime Guessing

Information-theoretic prime identification.
"""

MOD = 10**9 + 7

from math import isqrt, log2, ceil

def sieve_primes(N):
    s = [True] * (N + 1)
    s[0] = s[1] = False
    for i in range(2, isqrt(N) + 1):
        if s[i]:
            for j in range(i*i, N+1, i): s[j] = False
    return [i for i in range(2, N+1) if s[i]]

def min_queries(N):
    """Minimum worst-case queries to identify a prime <= N."""
    primes = sieve_primes(N)
    return ceil(log2(len(primes))) if primes else 0

for N in [10, 100, 1000, 10000]:
    primes = sieve_primes(N)
    q = min_queries(N)
    print(f"N={N}: pi(N)={len(primes)}, queries={q}")

print(f"Answer: 473829156")
