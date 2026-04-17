"""
Problem 741: Binary Grid Colouring

n x n grid, 2 blacks per row and column. g(n) = count up to symmetry. Burnside's lemma.
"""

MOD = 10**9 + 7

def f_small(n):
    """Count n x n binary matrices with row/col sums = 2, via brute force for small n."""
    from itertools import combinations
    # Each row chooses 2 columns
    rows = list(combinations(range(n), 2))
    count = 0

    def backtrack(row, col_counts):
        nonlocal count
        if row == n:
            if all(c == 2 for c in col_counts):
                count += 1
            return
        for c1, c2 in rows:
            if col_counts[c1] < 2 and col_counts[c2] < 2:
                col_counts[c1] += 1
                col_counts[c2] += 1
                backtrack(row + 1, col_counts)
                col_counts[c1] -= 1
                col_counts[c2] -= 1

    backtrack(0, [0] * n)
    return count

# Verify
print(f"f(4) = {f_small(4)}")  # Expected: 90
print(f"f(5) = {f_small(5)}")
print(f"f(6) = {f_small(6)}")

# Burnside for g(n) requires computing fixed points under D_4 symmetries
# For small n, we can enumerate and check symmetry
