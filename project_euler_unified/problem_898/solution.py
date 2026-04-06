"""
Problem 898: Product Partition Function
Count unordered multiplicative partitions (factorizations) of n.
f(n) = number of ways to write n as unordered product of factors > 1.
"""

from functools import lru_cache
from math import isqrt

def factorizations(n, min_factor=2):
    """Count unordered factorizations of n with all factors >= min_factor."""
    count = 1  # n itself (if n >= min_factor)
    d = min_factor
    while d * d <= n:
        if n % d == 0:
            count += factorizations(n // d, d)
        d += 1
    return count

@lru_cache(maxsize=None)
def f_cached(n, m=2):
    """Memoized version."""
    if n < m:
        return 0
    count = 1  # n itself as a single factor
    d = m
    while d * d <= n:
        if n % d == 0:
            count += f_cached(n // d, d)
        d += 1
    return count

def f_sieve(N):
    """Sieve-based computation of f(n) for all n up to N."""
    # g[n] = number of factorizations of n with smallest factor >= 2
    g = [0] * (N + 1)
    g[1] = 1
    for n in range(2, N + 1):
        g[n] = 1  # n itself
        d = 2
        while d * d <= n:
            if n % d == 0:
                # Add factorizations starting with d, recursively
                # but this requires the min_factor constraint
                pass
            d += 1
    # Simpler: just use the cached version
    return [f_cached(n) for n in range(N + 1)]

def get_divisors(n):
    """Return all divisors of n."""
    divs = []
    for d in range(1, isqrt(n) + 1):
        if n % d == 0:
            divs.append(d)
            if d != n // d:
                divs.append(n // d)
    return sorted(divs)

# --- Verification ---
print("=== Multiplicative Partitions f(n) ===")
print(f"{'n':>4} {'f(n)':>6}  Factorizations")
test_cases = {
    2: 1, 4: 2, 6: 2, 8: 3, 12: 4, 16: 5, 24: 7, 36: 9, 48: 12, 64: 15
}
for n in sorted(test_cases.keys()):
    fn = f_cached(n)
    expected = test_cases[n]
    status = "OK" if fn == expected else f"FAIL(expected {expected})"
    print(f"{n:>4} {fn:>6}  {status}")
    assert fn == expected, f"f({n})={fn} != {expected}"

# List factorizations of 12 explicitly
print("\n=== Factorizations of 12 ===")
def list_factorizations(n, min_f=2, prefix=()):
    results = []
    results.append(prefix + (n,))
    d = min_f
    while d * d <= n:
        if n % d == 0:
            results.extend(list_factorizations(n // d, d, prefix + (d,)))
        d += 1
    return results

for fz in list_factorizations(12):
    print(f"  {' x '.join(map(str, fz))}")

# Prime power verification: f(p^a) = p(a), the partition function
print("\n=== Prime Power: f(2^a) vs partition p(a) ===")
partitions = {1: 1, 2: 2, 3: 3, 4: 5, 5: 7, 6: 11, 7: 15}
for a in range(1, 8):
    fn = f_cached(2 ** a)
    pa = partitions[a]
    print(f"  f(2^{a}) = {fn}, p({a}) = {pa}, Match: {'OK' if fn == pa else 'FAIL'}")
    assert fn == pa

# Cumulative sum
print("\n=== Cumulative Sum ===")
for N in [10, 20, 50, 100, 200, 500]:
    s = sum(f_cached(n) for n in range(2, N + 1))
    print(f"  sum_{{n=2}}^{{{N}}} f(n) = {s}")

answer = sum(f_cached(n) for n in range(2, 1001))
print(f"\nAnswer: sum_{{n=2}}^{{1000}} f(n) = {answer}")

# --- 4-Panel Visualization ---
