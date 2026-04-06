"""
Problem 304: Primonacci

Find sum of F(p_{10^14+i}) mod 1234567891011 for i=1..100000,
where p_k is the k-th prime and F(n) is the n-th Fibonacci number.

Strategy:
1. Use sympy for prime counting and generation near 10^14-th prime.
2. Use matrix exponentiation for Fibonacci mod m.
3. Incremental computation via gap-based matrix multiplication.
"""

import sys
from sympy import nextprime, primepi, prime

MOD = 1234567891011

def mat_mul(A, B, m):
    """Multiply two 2x2 matrices mod m."""
    return [
        [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % m,
         (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % m],
        [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % m,
         (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % m]
    ]

def mat_pow(M, n, m):
    """Compute M^n mod m for 2x2 matrix."""
    R = [[1, 0], [0, 1]]  # identity
    while n > 0:
        if n & 1:
            R = mat_mul(R, M, m)
        M = mat_mul(M, M, m)
        n >>= 1
    return R

def fib_mod(n, m):
    """Compute F(n) mod m using matrix exponentiation."""
    if n <= 0:
        return 0
    if n <= 2:
        return 1 % m
    M = [[1, 1], [1, 0]]
    R = mat_pow(M, n - 1, m)
    return R[0][0]

def solve():
    target = 10**14

    # Find the (10^14)th prime using sympy
    # This may be slow for such large indices; sympy's prime() uses efficient methods
    # but may struggle at 10^14. We'll use an approximation + nextprime.

    # Known: the 10^14-th prime is approximately 3204941750802088
    # (from tables of prime counting function values)
    # We use a binary search approach with primepi if available,
    # or use the known value directly.

    # For practical computation, we use the known approximation:
    # pi(3204941750802048) is very close to 10^14
    # We'll search near this value.

    # Actually, let's try sympy's prime function for moderate sizes
    # and use manual search for large sizes.

    # Use known value: p_{10^14} ~ 3204941750802088 (approximate)
    # We'll verify with primepi and adjust.

    # For a Python solution that actually runs, we need primepi for large values.
    # sympy.primepi uses the Meissel-Lehmer algorithm.

    approx = 3204941750802088

    # Adjust: find exact p_{10^14} by checking pi near the approximation
    pc = primepi(approx)

    # Binary search for exact p_{10^14}
    if pc < target:
        # Need to go higher
        lo, hi = approx, approx + 10**8
        while lo < hi:
            mid = (lo + hi) // 2
            if primepi(mid) < target:
                lo = mid + 1
            else:
                hi = mid
    elif pc > target:
        lo, hi = approx - 10**8, approx
        while lo < hi:
            mid = (lo + hi) // 2
            if primepi(mid) < target:
                lo = mid + 1
            else:
                hi = mid
    else:
        lo = approx

    # lo is the smallest number with pi(lo) >= target
    # The target-th prime is the largest prime <= lo
    # Actually, lo might not be prime. The target-th prime p satisfies pi(p) = target.

    # Find the actual prime
    p_target = lo
    while primepi(p_target) > target:
        p_target -= 1
    while primepi(p_target) < target:
        p_target += 1
    # Now p_target is the smallest number with pi(p_target) = target
    # But we want it to be prime: the target-th prime.
    # Actually if pi(p_target) = target, then p_target might not be prime itself,
    # but the target-th prime is the largest prime <= p_target.

    # Let's just use nextprime to find primes starting after p_{10^14}
    # We need p_{10^14+1}, ..., p_{10^14+100000}

    # Find p_{10^14}: it's the largest prime <= p_target
    # If p_target is prime and pi(p_target) == target, then p_target = p_{target}
    # Otherwise, go back to the previous prime.

    from sympy import isprime, prevprime
    if not isprime(p_target):
        p_tc = prevprime(p_target + 1)
    else:
        p_tc = p_target

    # Now collect the next 100000 primes
    primes_list = []
    p = p_tc
    for _ in range(100000):
        p = nextprime(p)
        primes_list.append(p)

    # Compute sum of F(p_i) mod MOD
    M = [[1, 1], [1, 0]]

    # First prime: full matrix exponentiation
    cur = mat_pow(M, primes_list[0] - 1, MOD)
    total = cur[0][0] % MOD

    # Subsequent primes: incremental
    for i in range(1, len(primes_list)):
        gap = primes_list[i] - primes_list[i-1]
        Mgap = mat_pow(M, gap, MOD)
        cur = mat_mul(cur, Mgap, MOD)
        total = (total + cur[0][0]) % MOD

    print(total)

solve()
