"""
Problem 905: Lattice Point Visibility

Count visible lattice points (a,b) with 1 <= a,b <= N from the origin,
where visibility means gcd(a,b) = 1.  Find V(10^6) mod 10^9+7.

Key formula: V(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2

Methods:
    1. Mobius sieve + direct summation - O(N log log N)
    2. Euler totient summation (cross-check for small N)
    3. Brute force gcd check (verification for small N)
"""

from math import gcd

MOD = 10**9 + 7

# Mobius sieve (linear sieve)
def mobius_sieve(n: int) -> list:
    """Compute mu(k) for k = 0, 1, ..., n via linear sieve."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_prime = [True] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            mu[i * p] = -mu[i]
    return mu

def visible_count_mobius(N: int, mod: int):
    """V(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2 mod p."""
    mu = mobius_sieve(N)
    total = 0
    for d in range(1, N + 1):
        if mu[d] != 0:
            q = N // d
            total = (total + mu[d] * q * q) % mod
    return total % mod

def visible_count_totient(N: int):
    """V(N) = 2 * sum_{k=1}^{N} phi(k) - 1."""
    phi = list(range(N + 1))
    for p in range(2, N + 1):
        if phi[p] == p:  # p is prime
            for multiple in range(p, N + 1, p):
                phi[multiple] -= phi[multiple] // p
    return 2 * sum(phi[1:]) - 1

def visible_count_brute(N: int):
    """Count pairs (a,b) with gcd(a,b) = 1 by direct computation."""
    count = 0
    for a in range(1, N + 1):
        for b in range(1, N + 1):
            if gcd(a, b) == 1:
                count += 1
    return count

# Solve
N = 10**6

ans = visible_count_mobius(N, MOD)

# Verify on small cases with totient
for test_n in [5, 10, 50, 100]:
    v_mob = visible_count_mobius(test_n, 10**18)
    v_tot = visible_count_totient(test_n)
    assert v_mob == v_tot, f"N={test_n}: Mobius={v_mob}, Totient={v_tot}"

# Small brute force
for test_n in [5, 10]:
    v_brute = visible_count_brute(test_n)
    v_mob = visible_count_mobius(test_n, 10**18)
    assert v_brute == v_mob, f"N={test_n}: brute={v_brute}, Mobius={v_mob}"

print(ans)
