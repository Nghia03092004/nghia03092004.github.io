"""
Problem 489: Common Factors Between Two Sequences
Compute gcd(phi(n), sigma(n)) for positive integers n and analyze the results.
phi(n) = Euler's totient function, sigma(n) = sum of divisors.
"""

from math import gcd

def totient_sieve(limit: int) -> list:
    """Compute phi(n) for n = 0..limit using Euler's product sieve."""
    phi = list(range(limit + 1))
    for p in range(2, limit + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, limit + 1, p):
                phi[m] = phi[m] // p * (p - 1)
    return phi

def sigma_sieve(limit: int) -> list:
    """Compute sigma(n) = sum of divisors for n = 0..limit."""
    sigma = [0] * (limit + 1)
    for j in range(1, limit + 1):
        for m in range(j, limit + 1, j):
            sigma[m] += j
    return sigma

def solve(limit: int):
    """Compute sum of gcd(phi(n), sigma(n)) for n = 1..limit."""
    phi = totient_sieve(limit)
    sig = sigma_sieve(limit)
    total = 0
    for n in range(1, limit + 1):
        total += gcd(phi[n], sig[n])
    return total

# Compute for small values and display
limit = 100
phi = totient_sieve(limit)
sig = sigma_sieve(limit)

print("n | phi(n) | sigma(n) | gcd(phi,sigma)")
print("-" * 45)
for n in range(1, 21):
    g = gcd(phi[n], sig[n])
    print(f"{n:2d} | {phi[n]:5d}  | {sig[n]:6d}   | {g}")

answer_100 = solve(100)
print(f"\nSum of gcd(phi(n), sigma(n)) for n=1..100: {answer_100}")

answer_1000 = solve(1000)
print(f"Sum of gcd(phi(n), sigma(n)) for n=1..1000: {answer_1000}")
