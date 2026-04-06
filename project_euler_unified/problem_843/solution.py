"""
Problem 843: Periodic Tilings

Count domino tilings of an m x n grid.
Methods: Transfer matrix DP, Kasteleyn formula, Fibonacci recurrence (m=2).
"""

from math import cos, pi, sqrt
from functools import lru_cache

# --- Method 1: Transfer matrix DP ---
def tilings_dp(m: int, n: int):
    """Count tilings via profile DP. Profile = bitmask of m bits."""
    if m * n % 2 == 1:
        return 0
    if m > n:
        m, n = n, m  # ensure m <= n for efficiency

    def fill_column(profile_in, profile_out, row, m):
        """Recursively fill one column given incoming profile."""
        if row == m:
            yield profile_out
            return
        if (profile_in >> row) & 1:
            # Cell already filled by horizontal domino from previous column
            yield from fill_column(profile_in, profile_out, row + 1, m)
        else:
            # Place horizontal domino (extends to next column)
            yield from fill_column(profile_in, profile_out | (1 << row), row + 1, m)
            # Place vertical domino (if next row available and not filled)
            if row + 1 < m and not ((profile_in >> (row + 1)) & 1):
                yield from fill_column(profile_in, profile_out, row + 2, m)

    # DP over columns
    dp = {0: 1}  # profile -> count
    for col in range(n):
        new_dp = {}
        for profile_in, count in dp.items():
            for profile_out in fill_column(profile_in, 0, 0, m):
                new_dp[profile_out] = new_dp.get(profile_out, 0) + count
        dp = new_dp

    return dp.get(0, 0)

# --- Method 2: Kasteleyn formula ---
def tilings_kasteleyn(m: int, n: int):
    """Kasteleyn's closed-form product formula."""
    if m * n % 2 == 1:
        return 0
    product = 1.0
    for j in range(1, m // 2 + 1 + (m % 2)):
        for k in range(1, n // 2 + 1 + (n % 2)):
            val = 4 * cos(pi * j / (m + 1))**2 + 4 * cos(pi * k / (n + 1))**2
            product *= val
    return round(product)

# --- Method 3: Fibonacci for m=2 ---
def tilings_fib(n: int):
    """T(2, n) = F_{n+1}."""
    if n == 0:
        return 1
    a, b = 1, 1
    for _ in range(n - 1):
        a, b = b, a + b
    return b

# --- Verify ---
# m=2 cases
for n in range(1, 15):
    dp_val = tilings_dp(2, n)
    fib_val = tilings_fib(n)
    assert dp_val == fib_val, f"m=2, n={n}: DP={dp_val}, Fib={fib_val}"

# General cases
test_cases = [(2, 2, 2), (2, 3, 3), (2, 4, 5), (3, 4, 11), (4, 4, 36), (4, 6, 281)]
for m, n, expected in test_cases:
    dp_val = tilings_dp(m, n)
    assert dp_val == expected, f"T({m},{n}): got {dp_val}, expected {expected}"
    kast_val = tilings_kasteleyn(m, n)
    assert kast_val == expected, f"Kasteleyn T({m},{n}): got {kast_val}, expected {expected}"

print("All verification passed!")

answer = tilings_dp(8, 8)
print(f"T(8,8) = {answer}")
