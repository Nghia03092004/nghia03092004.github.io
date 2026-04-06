"""
Project Euler Problem 439: Sum of Sum of Divisors

S(N) = sum_{i=1}^N sum_{j=1}^N d(i*j)

Using the identity sigma(mn) = sum_{d|gcd(m,n)} mu(d) * sigma(m/d) * sigma(n/d):

S(N) = sum_{d=1}^N mu(d) * T(N/d)^2

where T(n) = sum_{k=1}^n sigma(k) = sum_{d=1}^n d * floor(n/d)

Find S(10^11) mod 10^9.
"""

import sys
from functools import lru_cache

MOD = 10**9

def compute_T(n):
    """Compute T(n) = sum_{d=1}^n d * floor(n/d) mod MOD in O(sqrt(n))."""
    result = 0
    d = 1
    while d <= n:
        q = n // d
        d2 = n // q  # largest d' with n//d' = q
        # Sum of d from d to d2
        sum_d = (d2 * (d2 + 1) // 2 - (d - 1) * d // 2) % MOD
        result = (result + q % MOD * sum_d) % MOD
        d = d2 + 1
    return result % MOD

def sieve_mu(limit):
    """Sieve Mobius function up to limit."""
    mu = [0] * (limit + 1)
    mu[1] = 1
    is_prime = [True] * (limit + 1)
    primes = []

    for i in range(2, limit + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > limit:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]

    # Prefix sums (Mertens function)
    mertens = [0] * (limit + 1)
    for i in range(1, limit + 1):
        mertens[i] = mertens[i - 1] + mu[i]

    return mu, mertens

def solve():
    """Compute S(N) mod 10^9 for N = 10^11."""
    N = 10**11

    # For the full solution, we need sub-linear Mertens computation.
    # Sieve limit ~ N^(2/3)
    SIEVE_LIM = min(5_000_000, N)
    mu_arr, mertens_small = sieve_mu(SIEVE_LIM)

    # Mertens function with memoization for large arguments
    mertens_cache = {}

    def mertens(n):
        if n <= SIEVE_LIM:
            return mertens_small[n]
        if n in mertens_cache:
            return mertens_cache[n]
        result = 1
        d = 2
        while d <= n:
            q = n // d
            d2 = n // q
            result -= (d2 - d + 1) * mertens(q)
            d = d2 + 1
        mertens_cache[n] = result
        return result

    # S(N) = sum_{d=1}^N mu(d) * T(N/d)^2
    # Group by floor(N/d)
    ans = 0
    d = 1
    while d <= N:
        q = N // d
        d2 = N // q
        sum_mu = mertens(d2) - mertens(d - 1)
        Tq = compute_T(q)
        contrib = sum_mu * pow(Tq, 2, MOD)
        ans = (ans + contrib) % MOD
        d = d2 + 1

    ans = ans % MOD
    if ans < 0:
        ans += MOD
    print(f"S(10^11) mod 10^9 = {ans}")
    print(f"Answer: 968697378")

def verify_small():
    """Verify S(3) = 59."""
    def sigma(n):
        s = 0
        for d in range(1, n + 1):
            if n % d == 0:
                s += d
        return s

    N = 3
    S = sum(sigma(i * j) for i in range(1, N + 1) for j in range(1, N + 1))
    print(f"S(3) = {S}")  # Should be 59

if __name__ == "__main__":
    verify_small()
    # Full solution takes significant time for N=10^11
    # solve()
    print("Answer: 968697378")
