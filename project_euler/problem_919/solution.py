"""
Problem 919: Multiplicative Persistence

The multiplicative persistence of n is the number of times you multiply
digits of n until reaching a single digit. Find sum of all n < 10^7
with persistence exactly 4.

Key ideas:
    - f(n) = product of digits of n; iterate until single digit.
    - For n >= 10, f(n) < n, so persistence is finite.
    - Numbers with digit 0 have persistence 1 (skip for target 4).
    - Erdos conjecture: persistence = O(log log n).
    - Smallest n with persistence 11: 277777788888899.

Methods:
    1. Direct computation for each n in [10, 10^7)
    2. Optimized: skip numbers containing digit 0
"""

from collections import Counter

# Core functions
def digit_product(n: int) -> int:
    """Product of digits of n."""
    p = 1
    while n > 0:
        p *= n % 10
        n //= 10
    return p

def persistence(n: int) -> int:
    """Multiplicative persistence of n."""
    count = 0
    while n >= 10:
        n = digit_product(n)
        count += 1
    return count

def solve(N: int = 10**7, target: int = 4) -> int:
    """Sum of all n < N with multiplicative persistence = target."""
    total = 0
    for n in range(10, N):
        if persistence(n) == target:
            total += n
    return total

def solve_optimized(N: int = 10**7, target: int = 4) -> int:
    """Same as solve but skips n with digit 0 (persistence 1)."""
    total = 0
    for n in range(10, N):
        # Quick check: if any digit is 0, persistence is 1
        temp = n
        has_zero = False
        while temp > 0:
            if temp % 10 == 0:
                has_zero = True
                break
            temp //= 10
        if has_zero:
            continue
        if persistence(n) == target:
            total += n
    return total

# Solve
answer = solve()
print(answer)

# Verification
# Known smallest numbers with each persistence
smallest_known = {0: 0, 1: 10, 2: 25, 3: 39, 4: 77, 5: 679, 6: 6788, 7: 68889}
for pers, val in smallest_known.items():
    assert persistence(val) == pers, f"persistence({val}) should be {pers}"

# Verify persistence(77) = 4: 77 -> 49 -> 36 -> 18 -> 8
assert digit_product(77) == 49
assert digit_product(49) == 36
assert digit_product(36) == 18
assert digit_product(18) == 8
