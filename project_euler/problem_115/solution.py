"""
Problem 115: Counting Block Combinations II

Find the least n for which F(50, n) > 1,000,000 where F(m, n) counts
ways to fill a row of length n with red blocks of minimum length m,
separated by at least one black square.

Recurrence: F(m, n) = F(m, n-1) + P(n-m-1)
where P(k) = sum of F(m, j) for j = -1 to k.
"""

# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
def fill_count(m: int, target: int):
    """Find least n where F(m, n) > target. Returns (n, {n: F(m,n)})."""
    f = {-1: 1, 0: 1}
    prefix = {-1: 1, 0: 2}  # prefix[k] = sum of f[-1]..f[k]

    n = 0
    while True:
        n += 1
        p_idx = n - m - 1
        p = prefix.get(p_idx, 0) if p_idx >= -1 else 0
        f[n] = f[n - 1] + p
        prefix[n] = prefix[n - 1] + f[n]

        if f[n] > target:
            return n, f

# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
def fill_count_brute(m: int, n: int):
    """Count valid configurations by recursion."""
    if n < 0:
        return 1 if n == -1 else 0
    if n == 0:
        return 1
    # Place black at position n
    count = fill_count_brute(m, n - 1)
    # Place red block of length L ending at position n
    for L in range(m, n + 1):
        count += fill_count_brute(m, n - L - 1)
    return count

# ---------------------------------------------------------------------------
# Verify and solve
# ---------------------------------------------------------------------------
# Cross-check with brute force for small cases
for n_test in range(15):
    assert fill_count_brute(3, n_test) == fill_count(3, 10**18)[1].get(n_test, 1), \
        f"Mismatch at F(3, {n_test})"

# Verify known values
_, f3 = fill_count(3, 10**18)
assert f3[7] == 17, f"F(3,7) should be 17, got {f3[7]}"

# Verify F(10, n) first exceeds 10^6 at n=57
n10, _ = fill_count(10, 1_000_000)
assert n10 == 57, f"F(10, n) exceeds 10^6 at n={n10}, expected 57"

# Main computation
answer, f50 = fill_count(50, 1_000_000)
assert answer == 168, f"Expected 168, got {answer}"
print(answer)

# ---------------------------------------------------------------------------
# 4-Panel Visualization
# ---------------------------------------------------------------------------
