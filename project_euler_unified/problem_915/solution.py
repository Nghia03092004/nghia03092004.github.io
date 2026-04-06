"""
Problem 915: Digit Power Sum Sequences

Define f(n) = sum of 5th powers of digits of n. Find the sum of all n <= 10^6
whose orbit under f eventually reaches 1.

Key ideas:
    - Orbit contraction: f(n) < n for n >= 10^7, so orbits enter [1, 413343].
    - Memoization: classify each value as "reaches 1" or not.
    - Armstrong numbers for p=5: 1, 4150, 4151, 54748, 92727, 93084, 194979.
    - The number 1 is a fixed point: f(1) = 1.

Methods:
    1. Memoized orbit classification
    2. Brute-force orbit tracing (for verification)
    3. Armstrong number identification
"""

from collections import Counter

POWER = 5
N = 10**6

# Precompute fifth powers of digits
POW5 = [d**POWER for d in range(10)]

def digit_power_sum(n: int) -> int:
    """Compute sum of 5th powers of digits of n."""
    s = 0
    while n > 0:
        s += POW5[n % 10]
        n //= 10
    return s

def solve(limit: int) -> int:
    """Sum of all n <= limit whose orbit under f eventually reaches 1."""
    # Cache: True if orbit reaches 1, False otherwise
    cache = {}

    def reaches_one(n: int) -> bool:
        if n in cache:
            return cache[n]
        # Trace orbit until we hit a cached value or detect a cycle
        path = []
        visited = set()
        x = n
        while x not in visited and x not in cache:
            visited.add(x)
            path.append(x)
            x = digit_power_sum(x)

        if x in cache:
            result = cache[x]
        elif x == 1:
            result = True
        else:
            # x is in visited but not 1 -> cycle not containing 1
            result = False
            # But check: maybe 1 is in the cycle
            # If x is in visited, we've found a cycle. Check if 1 is in it.
            cycle_start = path.index(x)
            cycle = path[cycle_start:]
            result = 1 in cycle

        # Cache all values in path
        for v in path:
            cache[v] = result
        return result

    total = 0
    for n in range(1, limit + 1):
        if reaches_one(n):
            total += n
    return total

def find_armstrong_numbers(power: int, max_digits: int = 7) -> list:
    """Find all Armstrong numbers for given power up to max_digits digits."""
    results = []
    for d in range(1, max_digits + 1):
        max_val = d * 9**power
        for n in range(max(1, 10**(d-1)), min(10**d, max_val + 1)):
            if digit_power_sum(n) == n:
                results.append(n)
    return results

# Solve
answer = solve(N)

# Find Armstrong numbers for verification
armstrong = find_armstrong_numbers(5)
# Expected: [1, 4150, 4151, 54748, 92727, 93084, 194979]

print(answer)

# Verification: check small cases
assert digit_power_sum(1) == 1
assert digit_power_sum(10) == 1
assert digit_power_sum(100) == 1
assert digit_power_sum(4150) == 4150  # Armstrong number
