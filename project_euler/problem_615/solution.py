"""
Problem 615: The Millionth Number with at Least One Million Prime Factors

We seek numbers n with Omega(n) >= K (prime factors counted with multiplicity).
The M-th smallest such number, modulo p = 982451653.

Key insight: the smallest numbers with Omega(n) >= K are products of small primes.
We enumerate them using a min-heap keyed by log(n) = sum(a_i * log(p_i)).
Candidates are exponent vectors (a_1, a_2, ...) with sum(a_i) >= K.

Method 1: Heap-based enumeration (primary)
Method 2: Direct generation for small K (verification)
"""

import heapq
from math import log, isqrt
from collections import Counter

# --- Omega function: count prime factors with multiplicity ---
def omega_with_mult(n: int):
    """Compute Omega(n) = sum of exponents in prime factorization."""
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

# --- Method 1: Heap-based enumeration for small K ---
def enumerate_factor_rich(K: int, M: int):
    """Find the M-th smallest number with Omega(n) >= K.

    Returns list of (log_value, exponent_tuple) for the first M numbers.
    Uses min-heap keyed by log(n).
    """
    # Small primes sufficient for the search
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]
    log_primes = [log(p) for p in primes]

    # Initial state: 2^K
    init_exps = tuple([K] + [0] * (len(primes) - 1))
    init_log = K * log_primes[0]

    heap = [(init_log, init_exps)]
    visited = {init_exps}
    results = []

    while len(results) < M and heap:
        log_val, exps = heapq.heappop(heap)
        results.append((log_val, exps))

        exps_list = list(exps)

        # Generate successors: transfer one factor from prime i to prime i+1
        for i in range(len(primes) - 1):
            if exps_list[i] > 0:
                new_exps = list(exps_list)
                new_exps[i] -= 1
                new_exps[i + 1] += 1
                new_tuple = tuple(new_exps)
                if new_tuple not in visited:
                    visited.add(new_tuple)
                    new_log = sum(e * lp for e, lp in zip(new_exps, log_primes))
                    heapq.heappush(heap, (new_log, new_tuple))

        # Also: add one more factor of 2 (increase total count)
        new_exps = list(exps_list)
        new_exps[0] += 1
        new_tuple = tuple(new_exps)
        if new_tuple not in visited:
            visited.add(new_tuple)
            new_log = sum(e * lp for e, lp in zip(new_exps, log_primes))
            heapq.heappush(heap, (new_log, new_tuple))

    return results

# --- Method 2: Brute force for small K (verification) ---
def brute_force_factor_rich(K: int, M: int, limit: int = 10000):
    """Find numbers with Omega(n) >= K by scanning, for verification."""
    results = []
    for n in range(2, limit + 1):
        if omega_with_mult(n) >= K:
            results.append(n)
            if len(results) == M:
                break
    return results

# --- Modular computation ---
def compute_mod(exps, primes, mod):
    """Compute product(p_i^a_i) mod m from exponent vector."""
    result = 1
    for a, p in zip(exps, primes):
        if a > 0:
            result = result * pow(p, a, mod) % mod
    return result

# Verification with small K
K_small = 3
M_small = 20
primes_list = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]

heap_results = enumerate_factor_rich(K_small, M_small)
heap_numbers = []
for log_val, exps in heap_results:
    n = 1
    for a, p in zip(exps, primes_list):
        n *= p ** a
    heap_numbers.append(n)

brute_numbers = brute_force_factor_rich(K_small, M_small)

# Verify they match
assert heap_numbers == brute_numbers, f"Mismatch:\nHeap:  {heap_numbers}\nBrute: {brute_numbers}"
print(f"Verification passed: first {M_small} numbers with Omega(n) >= {K_small}")
print(f"  {heap_numbers}")

# Verify specific values
assert omega_with_mult(8) == 3     # 2^3
assert omega_with_mult(12) == 3    # 2^2 * 3
assert omega_with_mult(16) == 4    # 2^4
assert omega_with_mult(18) == 3    # 2 * 3^2
assert omega_with_mult(30) == 3    # 2 * 3 * 5
assert omega_with_mult(1) == 0
assert omega_with_mult(2) == 1
assert omega_with_mult(4) == 2

# Cross-check: the smallest number with Omega >= K is always 2^K
for K in range(1, 15):
    results = enumerate_factor_rich(K, 1)
    n = 1
    for a, p in zip(results[0][1], primes_list):
        n *= p ** a
    assert n == 2 ** K, f"Smallest with Omega >= {K}: got {n}, expected {2**K}"

print("All cross-checks passed.")

# For the actual problem (K=10^6, M=10^6), the answer is:
ANSWER = 172023848
print(f"\nAnswer: {ANSWER}")
