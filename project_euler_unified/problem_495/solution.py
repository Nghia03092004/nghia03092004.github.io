"""
Problem 495: Writing n as the Product of k Distinct Divisors
Count the number of ways to write n as a product of exactly k distinct factors > 1.
"""

from functools import lru_cache
from collections import defaultdict

def get_divisors(n: int) -> list:
    """Return all divisors of n greater than 1, sorted."""
    divs = []
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            divs.append(i)
            if i != n // i:
                divs.append(n // i)
    divs.append(n)
    return sorted(set(divs))

@lru_cache(maxsize=None)
def W(n: int, k: int, max_factor: int):
    """Count ways to write n as product of k distinct factors, each in [2, max_factor]."""
    if k == 0:
        return 1 if n == 1 else 0
    if n < 2:
        return 0

    count = 0
    for d in get_divisors(n):
        if d > max_factor:
            break
        if d <= n // d or k == 1:  # ensure remaining product uses smaller factors
            count += W(n // d, k - 1, d - 1)
    return count

def total_factorizations(n: int):
    """S(n) = sum of W(n, k) for k = 1, 2, ..."""
    total = 0
    k = 1
    while True:
        w = W(n, k, n)
        if w == 0:
            break
        total += w
        k += 1
    return total

def solve_small(N: int = 100):
    """Compute sum of S(n) for n = 2 to N."""
    result = 0
    for n in range(2, N + 1):
        s = total_factorizations(n)
        result += s
    return result

# Compute for small values
print("Computing S(n) for small n:")
for n in range(2, 31):
    s = total_factorizations(n)
    if s > 1:
        print(f"  S({n}) = {s}")

total = solve_small(100)
print(f"\nSum of S(n) for n=2..100: {total}")

total_1000 = solve_small(1000)
print(f"Sum of S(n) for n=2..1000: {total_1000}")
