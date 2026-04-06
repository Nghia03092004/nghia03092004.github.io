"""
Problem 365: A Huge Binomial Coefficient

Compute C(10^18, 10^9) mod p for primes p in (1000, 5000) using Lucas' theorem.
Then sum C(10^18, 10^9) mod (p1*p2*p3) over all triples using CRT.

Answer: 162619462356610313
"""

from itertools import combinations

def sieve_primes(lo, hi):
    """Return list of primes in (lo, hi)."""
    is_prime = [True] * (hi + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(hi**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, hi + 1, i):
                is_prime[j] = False
    return [p for p in range(lo + 1, hi) if is_prime[p]]

def small_comb(n, k, p):
    """Compute C(n, k) mod p for n, k < p."""
    if k > n:
        return 0
    if k == 0 or k == n:
        return 1
    if k > n - k:
        k = n - k
    num = 1
    den = 1
    for i in range(k):
        num = num * ((n - i) % p) % p
        den = den * ((i + 1) % p) % p
    return num * pow(den, p - 2, p) % p

def lucas(n, k, p):
    """Compute C(n, k) mod p using Lucas' theorem."""
    result = 1
    while n > 0 or k > 0:
        ni, ki = n % p, k % p
        if ki > ni:
            return 0
        result = result * small_comb(ni, ki, p) % p
        n //= p
        k //= p
    return result

def crt3(r1, p1, r2, p2, r3, p3):
    """Chinese Remainder Theorem for three congruences."""
    M = p1 * p2 * p3
    M1, M2, M3 = p2 * p3, p1 * p3, p1 * p2
    inv1 = pow(M1, p1 - 2, p1)
    inv2 = pow(M2, p2 - 2, p2)
    inv3 = pow(M3, p3 - 2, p3)
    return (r1 * M1 * inv1 + r2 * M2 * inv2 + r3 * M3 * inv3) % M

def solve():
    N = 10**18
    K = 10**9

    primes = sieve_primes(1000, 5000)

    # Compute residues
    residues = {p: lucas(N, K, p) for p in primes}

    # Sum over all triples
    total = 0
    for p1, p2, p3 in combinations(primes, 3):
        val = crt3(residues[p1], p1, residues[p2], p2, residues[p3], p3)
        total += val

    return total

if __name__ == "__main__":
    answer = solve()
assert answer == 162619462356610313
print(answer)
    # Expected: 162619462356610313
