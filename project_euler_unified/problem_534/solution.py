"""
Problem 534: Weak Queens

A weak queen attacks only adjacent squares (king moves).
Place n non-attacking weak queens on an n x n board, one per row.
Constraint: queens in adjacent rows must be >= 2 columns apart.

Compute sum_{n=1}^{12} f(n) using bitmask DP.
"""

from collections import defaultdict

# --- Method 1: Bitmask DP ---
def f_bitmask(n: int):
    """Count placements via bitmask DP. O(n^2 * 2^n)."""
    if n == 0:
        return 0
    # dp[(mask, last_col)] = count
    dp = defaultdict(int)
    for c in range(n):
        dp[(1 << c, c)] = 1

    for row in range(1, n):
        new_dp = defaultdict(int)
        for (mask, prev_c), count in dp.items():
            if bin(mask).count('1') != row:
                continue
            for c in range(n):
                if mask & (1 << c):
                    continue
                if abs(c - prev_c) < 2:
                    continue
                new_dp[(mask | (1 << c), c)] += count
        for key, val in new_dp.items():
            dp[key] += val

    full_mask = (1 << n) - 1
    return sum(v for (mask, _), v in dp.items() if mask == full_mask)

# --- Method 2: Backtracking (verification) ---
def f_backtrack(n: int):
    """Count placements via backtracking."""
    count = [0]
    cols = []

    def solve(row):
        if row == n:
            count[0] += 1
            return
        for c in range(n):
            if c in cols:
                continue
            if row > 0 and abs(c - cols[-1]) < 2:
                continue
            cols.append(c)
            solve(row + 1)
            cols.pop()

    solve(0)
    return count[0]

# --- Compute and verify ---
results = {}
for n in range(1, 13):
    val = f_bitmask(n)
    results[n] = val
    if n <= 9:
        assert val == f_backtrack(n), f"Mismatch at n={n}: {val} vs {f_backtrack(n)}"
    print(f"f({n}) = {val}")

total = sum(results.values())
print(f"Sum = {total}")
assert total == 3884, f"Expected 3884, got {total}"
