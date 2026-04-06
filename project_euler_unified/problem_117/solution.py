"""
Problem 117: Red, Green, and Blue Tiles

Count ways to tile a row of 50 units using grey (1), red (2), green (3),
and blue (4) tiles, with colors freely mixed.

Recurrence: f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4) (tetranacci)
"""

# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
def solve_dp(n: int):
    """Compute f(n) using the tetranacci recurrence."""
    if n == 0: return 1
    f = [0] * (n + 1)
    f[0] = 1
    for i in range(1, n + 1):
        for L in range(1, min(4, i) + 1):
            f[i] += f[i - L]
    return f[n]

# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
def solve_brute(n: int):
    """Enumerate all valid tilings recursively."""
    if n == 0:
        return 1
    count = 0
    for L in range(1, min(4, n) + 1):
        count += solve_brute(n - L)
    return count

# ---------------------------------------------------------------------------
# Compute and verify
# ---------------------------------------------------------------------------
# Cross-check brute force and DP for small values
for n in range(16):
    assert solve_dp(n) == solve_brute(n), f"Mismatch at n={n}"

# Verify initial conditions
assert solve_dp(0) == 1
assert solve_dp(1) == 1
assert solve_dp(2) == 2
assert solve_dp(3) == 4
assert solve_dp(4) == 8

answer = solve_dp(50)
assert answer == 100808458960497, f"Expected 100808458960497, got {answer}"
print(answer)

# ---------------------------------------------------------------------------
# 4-Panel Visualization
# ---------------------------------------------------------------------------
ns = list(range(0, 51))
vals = [solve_dp(n) for n in ns]
