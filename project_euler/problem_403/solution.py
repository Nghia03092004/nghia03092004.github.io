#!/usr/bin/env python3
"""
Project Euler Problem 403: Lattice points enclosed by parabola and line

For integers a and b, the domain D(a,b) = {(x,y) | x^2 <= y <= ax + b}.
L(a,b) counts the lattice points in D(a,b).
S(N) = sum of L(a,b) for all (a,b) with |a|,|b| <= N such that
       the area of D(a,b) is rational.

Find S(10^12) mod 10^8.

Key mathematical insights:
1. Area of D(a,b) = (a^2+4b)^(3/2) / 6, which is rational iff a^2+4b is a perfect square.
2. Let a^2+4b = s^2 (s >= 0). Then L(a,b) depends only on s:
   L(s) = (s+1)(s^2 - s + 6) / 6
3. Substituting a = s + 2u gives b = u(s+u), reducing the problem to counting
   valid (u,s) pairs and summing L(s).
4. Using prefix sums P(K) = sum_{s=0}^{K} L(s) = (K+1)(K+2)(K^2-K+12)/24,
   each group of u values contributes a closed-form expression.
5. The floor(N/v) grouping trick reduces the O(N) loop to O(sqrt(N)).
"""

import math
from math import comb
import time

# ---------------------------------------------------------------------------
# Core mathematical functions
# ---------------------------------------------------------------------------

def L(s):
    """Number of lattice points for discriminant root s."""
    return (s + 1) * (s * s - s + 6) // 6

def P(K):
    """Prefix sum: sum_{s=0}^{K} L(s)."""
    if K < 0:
        return 0
    return (K + 1) * (K + 2) * (K * K - K + 12) // 24

def sum_pow(a, b, k):
    """Exact sum of v^k for v = a, a+1, ..., b."""
    if a > b:
        return 0

    def prefix(n):
        if n < 0:
            return 0
        if k == 0:
            return n + 1
        elif k == 1:
            return n * (n + 1) // 2
        elif k == 2:
            return n * (n + 1) * (2 * n + 1) // 6
        elif k == 3:
            t = n * (n + 1) // 2
            return t * t
        elif k == 4:
            return n * (n + 1) * (2 * n + 1) * (3 * n * n + 3 * n - 1) // 30

    return prefix(b) - prefix(a - 1)

def sum_P_linear(v1, v2, coeff, offset):
    """Compute sum_{v=v1}^{v2} P(coeff*v + offset) exactly.

    P(w) = (w^4 + 2w^3 + 11w^2 + 34w + 24) / 24.
    Substituting w = coeff*v + offset and expanding gives a degree-4
    polynomial in v whose partial sums are computable in O(1).
    """
    if v1 > v2:
        return 0
    c, d = coeff, offset

    def wk_coeff(k, j):
        if j > k:
            return 0
        return comb(k, j) * c ** j * d ** (k - j)

    total = 0
    for j in range(5):
        cj = (wk_coeff(4, j) + 2 * wk_coeff(3, j) +
              11 * wk_coeff(2, j) + 34 * wk_coeff(1, j) +
              24 * (1 if j == 0 else 0))
        total += cj * sum_pow(v1, v2, j)

    return total // 24

# ---------------------------------------------------------------------------
# Brute-force solver (for verification, small N only)
# ---------------------------------------------------------------------------

def L_bruteforce(a, b):
    """Count lattice points (x,y) with x^2 <= y <= ax + b."""
    disc = a * a + 4 * b
    if disc < 0:
        return 0
    sq = int(math.isqrt(disc))
    if sq * sq != disc:
        return 0
    r1 = (a - sq) // 2
    r2 = (a + sq) // 2
    count = 0
    for x in range(r1, r2 + 1):
        gap = a * x + b - x * x
        if gap >= 0:
            count += gap + 1
    return count

def S_bruteforce(N):
    """Brute-force S(N): enumerate all (a,b) with |a|,|b| <= N."""
    total = 0
    for a in range(-N, N + 1):
        for b in range(-N, N + 1):
            total += L_bruteforce(a, b)
    return total

# ---------------------------------------------------------------------------
# O(N) solver
# ---------------------------------------------------------------------------

def S_linear(N):
    """O(N) algorithm using the u-parametrization."""
    total = 0
    sq = int(math.isqrt(N))

    # u = 0: s in [0, N]
    total += P(N)

    # u >= 1: a = s + 2u, b = u(s+u), need |a| <= N and |b| <= N
    for u in range(1, sq + 1):
        s_hi = min((N - u * u) // u, N - 2 * u)
        if s_hi >= 0:
            total += P(s_hi)

    # v >= 1 (u = -v): a = s - 2v, b = -v(s-v)
    for v in range(1, sq + 1):
        s_hi = v + N // v
        s_lo = max(0, 2 * v - N)
        if s_hi >= s_lo:
            total += P(s_hi) - P(s_lo - 1)

    for v in range(sq + 1, N + 1):
        q = N // v
        s_hi = v + q
        s_lo = max(v - q, 2 * v - N, 0)
        if s_lo <= s_hi:
            total += P(s_hi) - P(s_lo - 1)

    return total

# ---------------------------------------------------------------------------
# O(sqrt(N)) solver
# ---------------------------------------------------------------------------

def S_fast(N):
    """O(sqrt(N)) algorithm using floor(N/v) grouping."""
    total = 0
    sq = int(math.isqrt(N))

    # Part 1: u = 0 => s in [0, N]
    total += P(N)

    # Part 2: u = 1 .. sqrt(N)
    for u in range(1, sq + 1):
        s_hi = min((N - u * u) // u, N - 2 * u)
        if s_hi >= 0:
            total += P(s_hi)

    # Part 3: v = 1 .. sqrt(N) (u = -v)
    for v in range(1, sq + 1):
        s_hi = v + N // v
        s_lo = max(0, 2 * v - N)
        if s_hi >= s_lo:
            total += P(s_hi) - P(s_lo - 1)

    # Part 4: v = sqrt(N)+1 .. N, grouped by q = floor(N/v)
    v = sq + 1
    while v <= N:
        q = N // v
        if q == 0:
            break
        v_end = min(N // q, N)
        v_cut = N - q

        v_start = v
        v_end_A = min(v_cut, v_end)
        v_start_B = max(v_start, v_cut + 1)
        v_end_B = v_end

        # Sub-range A: s_lo = v - q, s_hi = v + q
        if v_start <= v_end_A:
            total += (sum_P_linear(v_start, v_end_A, 1, q) -
                      sum_P_linear(v_start, v_end_A, 1, -q - 1))

        # Sub-range B: s_lo = 2v - N, s_hi = v + q
        if v_start_B <= v_end_B:
            total += (sum_P_linear(v_start_B, v_end_B, 1, q) -
                      sum_P_linear(v_start_B, v_end_B, 2, -N - 1))

        v = v_end + 1

    return total

# ---------------------------------------------------------------------------
# Verification and main computation
# ---------------------------------------------------------------------------

def verify():
    """Verify all implementations against known values."""
    print("=" * 60)
    print("Verification")
    print("=" * 60)

    # Check L examples
    assert L_bruteforce(1, 2) == 8, f"L(1,2) = {L_bruteforce(1,2)}, expected 8"
    assert L_bruteforce(2, -1) == 1, f"L(2,-1) = {L_bruteforce(2,-1)}, expected 1"
    print("L(1,2) = 8, L(2,-1) = 1  [OK]")

    # Check formula L(s) = (s+1)(s^2-s+6)/6
    assert L(3) == 8   # L(1,2): disc=9, s=3
    assert L(0) == 1   # L(2,-1): disc=0, s=0
    print("L formula matches brute force  [OK]")

    # Check S examples
    for N, expected in [(5, 344), (100, 26709528)]:
        bf = S_bruteforce(N) if N <= 100 else None
        lin = S_linear(N)
        fast = S_fast(N)
        if bf is not None:
            assert bf == expected, f"S_bruteforce({N}) = {bf}, expected {expected}"
        assert lin == expected, f"S_linear({N}) = {lin}, expected {expected}"
        assert fast == expected, f"S_fast({N}) = {fast}, expected {expected}"
        print(f"S({N}) = {expected}  [OK]")

    # Cross-check larger values
    for N in [1000, 5000]:
        lin = S_linear(N)
        fast = S_fast(N)
        assert lin == fast, f"Mismatch at N={N}: linear={lin}, fast={fast}"
        print(f"S({N}) = {fast}  [OK, linear and fast agree]")

    print()

def main():
    verify()

    print("=" * 60)
    print("Computing S(10^12) mod 10^8")
    print("=" * 60)

    N = 10 ** 12
    MOD = 10 ** 8

    t0 = time.time()
    result = S_fast(N) % MOD
    elapsed = time.time() - t0

    print(f"S(10^12) mod 10^8 = {result}")
    print(f"Time: {elapsed:.2f}s")

# ---------------------------------------------------------------------------
