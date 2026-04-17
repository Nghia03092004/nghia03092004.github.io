#!/usr/bin/env python3
"""
Project Euler Problem 452 — Long Products

F(m, n) = number of n-tuples (a1, ..., an) of positive integers with product <= m.
Compute F(10^9, 10^9) mod 1234567891.

Key insight:
  F(m, n) = sum_{k=1}^{m} d_n(k)

where d_n(k) = number of ordered factorizations of k into exactly n positive
integer factors.  d_n is multiplicative, and for a prime power p^a:
  d_n(p^a) = C(n + a - 1, a)

Since m = 10^9, the maximum exponent a for any prime is 29 (since 2^30 > 10^9).
C(n+a-1, a) is a polynomial of degree a in n, computable mod the prime modulus.

Strategy — recursive prime-power decomposition:
  We iterate over primes p up to m via a sieve.  For each prime power p^a <= m,
  we accumulate the contribution.  We use a "multiplicative prefix sum" approach:
  process primes one by one, maintaining partial sums over integers composed only
  of primes already processed.

  Concretely, let S(x, pi) = sum of d_n(k) over all k <= x whose smallest prime
  factor is > p_pi (the pi-th prime).  Then:
    F(m, n) = S(m, 0)
  with the recursion:
    S(x, pi) = 1 + sum_{p = next prime after p_pi}^{x} sum_{a=1}^{floor(log_p(x))}
               d_n(p^a) * S(x / p^a, index_of(p))

  The "1" accounts for k = 1 (all ones tuple, d_n(1) = 1).

  This is essentially Lucy_Hedgehog's method adapted for a general multiplicative
  function, implemented via memoized recursion with the key being x (since
  all values of x that appear are of the form floor(m / something), of which
  there are O(sqrt(m)) distinct values).

For m = n = 10^9 and MOD = 1234567891, this runs in roughly O(m^{2/3}) time
with appropriate sieve limits and memoization.

However, for the full 10^9 case, pure Python is too slow. This script solves
the smaller test cases and demonstrates the algorithm. The C++ solution handles
the full problem.
"""

import sys
import math
import time

MOD = 1234567891

def solve_small(m, n, mod=None):
    """
    Direct computation of F(m, n) for small m.
    Factorize each k in [1, m], compute d_n(k), sum them up.
    """
    # Sieve smallest prime factor
    spf = list(range(m + 1))
    for i in range(2, int(m**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, m + 1, i):
                if spf[j] == j:
                    spf[j] = i

    def d_n(k, n_val):
        """Compute d_n(k) = product of C(n+a-1, a) over prime powers p^a || k."""
        if k == 1:
            return 1
        result = 1
        while k > 1:
            p = spf[k]
            a = 0
            while k % p == 0:
                k //= p
                a += 1
            # C(n_val + a - 1, a)
            binom = 1
            for i in range(1, a + 1):
                binom = binom * (n_val + a - i) // i
            if mod:
                binom %= mod
            result = result * binom
            if mod:
                result %= mod
        return result

    total = 0
    for k in range(1, m + 1):
        total += d_n(k, n_val=n)
        if mod:
            total %= mod
    return total if not mod else total % mod

def modinv(a, m):
    """Modular inverse using extended Euclidean algorithm."""
    g, x, _ = extended_gcd(a % m, m)
    if g != 1:
        return None
    return x % m

def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    g, x, y = extended_gcd(b % a, a)
    return g, y - (b // a) * x, x

def binom_mod(n_val, a, mod):
    """Compute C(n_val + a - 1, a) mod `mod`. a is small (<=29)."""
    # C(n_val + a - 1, a) = product_{i=1}^{a} (n_val + a - i) / i
    num = 1
    den = 1
    for i in range(1, a + 1):
        num = num * ((n_val + a - i) % mod) % mod
        den = den * i % mod
    return num * modinv(den, mod) % mod

def solve_medium(m, n, mod=MOD):
    """
    Solve F(m, n) mod `mod` using prime-by-prime multiplicative prefix sum.

    We process primes in increasing order.  We maintain an array/dict where
    for each "bucket" value v (which is always of the form floor(m/k)),
    we store the current partial sum of d_n over all integers <= v composed
    only of primes we have already processed.

    Initially (before processing any prime), only k=1 qualifies, so every
    bucket has value 1.

    When we process prime p, for each bucket v (in decreasing order), and
    for each exponent a = 1, 2, ... while p^a <= v, we add:
      binom(n, a) * bucket_value[floor(v / p^a)]
    where binom(n, a) = C(n+a-1, a) is the d_n contribution of p^a.
    """
    if m <= 1:
        return 1 if m == 1 else 0

    # Generate all distinct values of floor(m / k)
    # These are O(2*sqrt(m)) values
    sqrt_m = int(m**0.5)
    vals = []
    for i in range(1, sqrt_m + 1):
        vals.append(m // i)
        vals.append(i)
    vals = sorted(set(vals))

    # Map value -> index
    # Use two arrays: for v <= sqrt_m, index directly; for v > sqrt_m, use m//v
    # which is <= sqrt_m.
    # S[v] = current partial sum for bucket v
    # We store S in a dict for simplicity (Python version)
    S = {}
    for v in vals:
        S[v] = 1  # initially only k=1 contributes

    # Sieve primes up to sqrt(m) (we need primes up to m, but primes > sqrt(m)
    # contribute only at exponent a=1)
    limit = m
    # For a full sieve up to m we need memory; use segmented or just sieve up
    # to sqrt(m) for the "small prime" part, then handle large primes.

    # Actually for this approach we sieve primes up to m.
    # For m up to ~10^6 this is fine in Python.  For 10^9 use C++.
    if m > 5_000_000:
        print(f"WARNING: m={m} is large for Python. Use C++ for full solution.")
        return None

    sieve = bytearray(b'\x01') * (m + 1)
    sieve[0] = sieve[1] = 0
    for i in range(2, int(m**0.5) + 1):
        if sieve[i]:
            for j in range(i * i, m + 1, i):
                sieve[j] = 0
    primes = [i for i in range(2, m + 1) if sieve[i]]

    # Precompute binom values: C(n+a-1, a) mod for a = 1..29
    max_a = int(math.log2(m)) + 1 if m >= 2 else 1
    binom_cache = [0] * (max_a + 1)
    for a in range(1, max_a + 1):
        binom_cache[a] = binom_mod(n, a, mod)

    # Process each prime
    for p in primes:
        # We iterate over bucket values in DECREASING order to avoid
        # counting the same prime twice (similar to knapsack)
        for v in reversed(vals):
            if v < p:
                break
            # For each exponent a = 1, 2, ... while p^a <= v
            pk = p  # p^a
            a = 1
            while pk <= v:
                # floor(v / p^a)
                v_div = v // pk
                S[v] = (S[v] + binom_cache[a] * S[v_div]) % mod
                if pk > v // p:
                    break
                pk *= p
                a += 1

    return S[m]

def verify():
    """Verify against known values."""
    print("Verifying F(10, 10)...")
    result = solve_small(10, 10)
    print(f"  F(10, 10) = {result}  (expected 571)")
    assert result == 571, f"FAILED: got {result}"

    print("Verifying with medium solver...")
    result2 = solve_medium(10, 10, MOD)
    print(f"  F(10, 10) via medium = {result2}  (expected 571)")
    assert result2 == 571, f"FAILED: got {result2}"

    # Test F(100, 100)
    r1 = solve_small(100, 100, MOD)
    r2 = solve_medium(100, 100, MOD)
    print(f"  F(100, 100) = {r1} (small) vs {r2} (medium)")
    assert r1 == r2, f"MISMATCH: {r1} vs {r2}"

    # Test F(1000, 1000)
    r1 = solve_small(1000, 1000, MOD)
    r2 = solve_medium(1000, 1000, MOD)
    print(f"  F(1000, 1000) = {r1} (small) vs {r2} (medium)")
    assert r1 == r2, f"MISMATCH: {r1} vs {r2}"

    print("All verifications passed!")

def create_visualization():
    """Create visualization of the problem structure."""
