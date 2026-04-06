"""
Problem 927: Totient Sum Optimization

Compute the sum of Euler's totient function phi(k) for k = 1..N, modulo 10^9+7.
Uses a sieve-based approach to compute all phi values efficiently.

Key ideas:
    - phi(n) = n * product(1 - 1/p) for each prime p dividing n.
    - Sieve of Eratosthenes variant computes phi for all n in O(N log log N).
    - Summatory totient: Phi(N) = sum_{k=1}^{N} phi(k) ~ 3N^2 / pi^2.
    - phi(n)/n ratio distribution reflects multiplicative structure.

Methods:
    1. Euler sieve for phi(1..N)
    2. Direct phi computation via factorization (verification)
    3. Ratio phi(n)/n distribution analysis
    4. Summatory function vs asymptotic approximation
"""

from math import gcd

def euler_sieve(N):
    """Compute phi(0..N) using multiplicative sieve."""
    phi = list(range(N + 1))
    for p in range(2, N + 1):
        if phi[p] == p:  # p is prime
            for m in range(p, N + 1, p):
                phi[m] -= phi[m] // p
    return phi

def solve(N=10**7):
    MOD = 10**9 + 7
    phi = euler_sieve(N)
    return sum(phi[1:]) % MOD

def phi_direct(n):
    """Compute phi(n) by counting coprime integers."""
    if n <= 1:
        return n
    return sum(1 for k in range(1, n + 1) if gcd(k, n) == 1)

def phi_factor(n):
    """Compute phi(n) from prime factorization."""
    result = n
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def summatory_totient(phi_list):
    """Compute cumulative sum of phi values."""
    cumsum = [0]
    for k in range(1, len(phi_list)):
        cumsum.append(cumsum[-1] + phi_list[k])
    return cumsum

# Solve and verify
answer = solve()

# Verify sieve against direct computation for small values
phi_small = euler_sieve(100)
for n in range(1, 101):
    assert phi_small[n] == phi_direct(n), f"Mismatch at n={n}"
    assert phi_small[n] == phi_factor(n), f"Factor mismatch at n={n}"

# Known values
assert phi_small[1] == 1
assert phi_small[2] == 1
assert phi_small[6] == 2
assert phi_small[12] == 4
assert phi_small[97] == 96  # prime

print(answer)
