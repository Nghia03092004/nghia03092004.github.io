"""
Problem 907: Minimum Path Sum in Triangle

Given a triangle of N=1000 rows with T(i,j) = ((i+1)*(j+1)*37 + 13) % 100,
find the minimum path sum from apex to base.

Bellman equation: V(i,j) = T(i,j) + min(V(i+1,j), V(i+1,j+1))

Methods:
    1. Bottom-up DP (in-place): O(N^2) time, O(N) space
    2. Top-down with memoization: O(N^2) time, O(N^2) space
    3. Brute-force DFS (small N verification): O(2^N) time
"""

# Triangle generation
def make_triangle(N: int) -> list:
    """Generate the triangle: T(i,j) = ((i+1)*(j+1)*37 + 13) % 100."""
    return [[(( i + 1) * (j + 1) * 37 + 13) % 100 for j in range(i + 1)]
            for i in range(N)]

def min_path_dp(tri: list):
    """Compute minimum path sum using bottom-up DP.

    dp[j] starts as bottom row, then updated upward:
    dp[j] = tri[i][j] + min(dp[j], dp[j+1])
    """
    N = len(tri)
    dp = tri[-1][:]
    for i in range(N - 2, -1, -1):
        for j in range(i + 1):
            dp[j] = tri[i][j] + min(dp[j], dp[j + 1])
    return dp[0]

def min_path_topdown(tri: list):
    """Compute minimum path sum using top-down memoization."""
    import sys
    sys.setrecursionlimit(10000)
    N = len(tri)
    memo = {}

    def dp(i, j):
        if i == N - 1:
            return tri[i][j]
        if (i, j) not in memo:
            memo[(i, j)] = tri[i][j] + min(dp(i + 1, j), dp(i + 1, j + 1))
        return memo[(i, j)]

    return dp(0, 0)

def min_path_brute(tri: list):
    """Find minimum path by exploring all 2^(N-1) paths."""
    N = len(tri)
    if N == 1:
        return tri[0][0]
    best = float("inf")

    def dfs(i, j, total):
        nonlocal best
        total += tri[i][j]
        if i == N - 1:
            best = min(best, total)
            return
        dfs(i + 1, j, total)
        dfs(i + 1, j + 1, total)

    dfs(0, 0, 0)
    return best

# Solve
N = 1000
tri = make_triangle(N)
answer = min_path_dp(tri)

# Verify with top-down for medium N
tri_med = make_triangle(100)
assert min_path_dp(tri_med) == min_path_topdown(tri_med)

# Verify with brute force for small N
for test_n in [3, 5, 8, 10]:
    tri_test = make_triangle(test_n)
    assert min_path_dp(tri_test) == min_path_brute(tri_test), \
        f"N={test_n}: DP != brute"

print(answer)
