"""
Problem 921: Divisor Chain Lengths

L(n) = maximum divisor chain length from n to 1 = Omega(n).
Find sum of L(n) for n = 1..10^5, mod 10^9+7.

Key ideas:
    - L(n) = Omega(n), the number of prime factors with multiplicity.
    - Each chain step divides by one prime; this is optimal.
    - Compute Omega(n) via sieve of prime powers.
    - Summation formula: sum Omega(n) = sum_{p prime} sum_k floor(N/p^k).

Methods:
    1. Sieve-based Omega computation
    2. Direct summation formula over primes
    3. Verification via factorization
"""

from collections import Counter

MOD = 10**9 + 7

def sieve_omega(N: int) -> list:
    """Compute Omega(n) for n = 0..N using a sieve."""
    omega = [0] * (N + 1)
    for p in range(2, N + 1):
        if omega[p] == 0:  # p is prime
            pk = p
            while pk <= N:
                for m in range(pk, N + 1, pk):
                    omega[m] += 1
                pk *= p
    return omega

def sum_omega_formula(N: int) -> int:
    """Compute sum_{n=1}^{N} Omega(n) = sum_{p prime} sum_k floor(N/p^k)."""
    # Sieve primes up to N
    is_prime = [True] * (N + 1)
    is_prime[0] = is_prime[1] = False
    for p in range(2, int(N**0.5) + 1):
        if is_prime[p]:
            for m in range(p*p, N + 1, p):
                is_prime[m] = False

    total = 0
    for p in range(2, N + 1):
        if is_prime[p]:
            pk = p
            while pk <= N:
                total += N // pk
                pk *= p
    return total

def omega_brute(n: int) -> int:
    """Compute Omega(n) by trial division."""
    count = 0
    d = 2
    while d * d <= n:
        while n % d == 0:
            count += 1
            n //= d
        d += 1
    if n > 1:
        count += 1
    return count

# Solve
N = 10**5
omega = sieve_omega(N)
answer = sum(omega[1:N+1]) % MOD

# Verify methods agree
assert sum(omega[1:N+1]) == sum_omega_formula(N)

# Verify Omega for small values
assert omega[1] == 0
assert omega[6] == 2   # 2 * 3
assert omega[8] == 3   # 2^3
assert omega[12] == 3  # 2^2 * 3
assert omega[60] == 4  # 2^2 * 3 * 5

# Verify against brute force for small n
for n in range(1, 1000):
    assert omega[n] == omega_brute(n), f"n={n}: sieve={omega[n]}, brute={omega_brute(n)}"

print(answer)
