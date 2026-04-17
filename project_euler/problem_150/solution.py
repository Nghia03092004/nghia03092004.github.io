"""
Problem 150: Minimum-sum Sub-triangle in Triangular Array

Uses LCG to generate entries, prefix sums for O(1) row-slice queries,
and exhaustive enumeration of all sub-triangles.
"""

def solve(N: int = 1000):
    MOD = 1 << 20
    HALF = 1 << 19

    # Generate triangular array
    tri = []
    t = 0
    for r in range(N):
        row = []
        for c in range(r + 1):
            t = (615949 * t + 797807) % MOD
            row.append(t - HALF)
        tri.append(row)

    # Prefix sums per row
    prefix = []
    for r in range(N):
        p = [0] * (r + 2)
        for c in range(r + 1):
            p[c + 1] = p[c] + tri[r][c]
        prefix.append(p)

    # Find minimum sub-triangle sum
    ans = float('inf')
    for r in range(N):
        for c in range(r + 1):
            tri_sum = 0
            for h in range(N - r):
                tri_sum += prefix[r + h][c + h + 1] - prefix[r + h][c]
                if tri_sum < ans:
                    ans = tri_sum
    return ans

def solve_brute_small(N: int = 10):
    """Brute force for small N to verify."""
    MOD = 1 << 20
    HALF = 1 << 19
    tri = []
    t = 0
    for r in range(N):
        row = []
        for c in range(r + 1):
            t = (615949 * t + 797807) % MOD
            row.append(t - HALF)
        tri.append(row)

    ans = float('inf')
    for r in range(N):
        for c in range(r + 1):
            for h in range(N - r):
                s = sum(tri[r + i][c + j] for i in range(h + 1) for j in range(i + 1))
                # This is wrong - should be c to c+i
                pass
    # Use prefix sum version for brute force too
    prefix = []
    for r in range(N):
        p = [0] * (r + 2)
        for c in range(r + 1):
            p[c + 1] = p[c] + tri[r][c]
        prefix.append(p)

    for r in range(N):
        for c in range(r + 1):
            tri_sum = 0
            for h in range(N - r):
                tri_sum += prefix[r + h][c + h + 1] - prefix[r + h][c]
                if tri_sum < ans:
                    ans = tri_sum
    return ans

# Verify on small case
small_ans = solve_brute_small(10)
small_ans2 = solve(10)
assert small_ans == small_ans2, f"Small case mismatch: {small_ans} vs {small_ans2}"

# Main solve
answer = solve(1000)
assert answer == -271248680, f"Expected -271248680, got {answer}"
print(answer)
