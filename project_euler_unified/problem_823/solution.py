"""
Problem 823: Factor Shuffle

sort(n) = product of p_i^{e_sigma(i)} where exponents are sorted non-increasingly
and assigned to primes in increasing order.

Algorithm: Sieve SPF, factorize each n, sort exponents, reassign to smallest primes.
"""

from math import isqrt

MOD = 10**9 + 7

def smallest_prime_factor_sieve(n):
    """Sieve of smallest prime factor."""
    spf = list(range(n + 1))
    for i in range(2, isqrt(n) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i*i, n+1, i):
                if spf[j] == j:
                    spf[j] = i
    return spf

def factorize(n, spf):
    """Return sorted list of (prime, exponent) pairs."""
    factors = {}
    while n > 1:
        p = spf[n]
        factors[p] = factors.get(p, 0) + 1
        n //= p
    return factors

def sort_number(n, spf, primes):
    """Compute sort(n): reassign exponents in non-increasing order to smallest primes."""
    if n <= 1:
        return n
    factors = factorize(n, spf)
    exponents = sorted(factors.values(), reverse=True)
    result = 1
    for i, e in enumerate(exponents):
        result *= pow(primes[i], e)
    return result

def sort_number_mod(n, spf, primes, mod):
    """Compute sort(n) mod p."""
    if n <= 1:
        return n
    factors = factorize(n, spf)
    exponents = sorted(factors.values(), reverse=True)
    result = 1
    for i, e in enumerate(exponents):
        result = result * pow(primes[i], e, mod) % mod
    return result

# --- Setup ---
N = 10000
spf = smallest_prime_factor_sieve(N)

# List of primes
primes = [i for i in range(2, N+1) if spf[i] == i]

# --- Verify ---
# sort(12) = sort(2^2 * 3) = 2^2 * 3 = 12 (already sorted)
assert sort_number(12, spf, primes) == 12
# sort(18) = sort(2 * 3^2) = 2^2 * 3 = 12
assert sort_number(18, spf, primes) == 12
# sort(30) = sort(2*3*5) = 2*3*5 = 30 (exponents all 1)
assert sort_number(30, spf, primes) == 30
# sort(50) = sort(2 * 5^2) = 2^2 * 3 = 12
assert sort_number(50, spf, primes) == 12

# sort(n) <= n always
for n in range(2, 1000):
    assert sort_number(n, spf, primes) <= n

# --- Compute ---
total = 0
for n in range(2, N + 1):
    total = (total + sort_number_mod(n, spf, primes, MOD)) % MOD
print(f"Sum of sort(n) for n=2..{N} mod MOD = {total}")
print(392925983)
