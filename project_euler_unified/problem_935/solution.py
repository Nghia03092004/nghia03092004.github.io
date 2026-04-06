"""
Problem 935: Lattice Paths with Obstacles

Count lattice paths from (0,0) to (M,M) on an MxM grid using only right
and up steps, avoiding blocked points. A point (x,y) is blocked when
x+y is a perfect square AND gcd(x,y) > 1. Answer mod 10^9+7.

Key observations:
  - Obstacles form a structured pattern based on number-theoretic conditions.
  - Standard DP on the grid: dp[x][y] = dp[x-1][y] + dp[x][y-1], with
    dp[x][y] = 0 for blocked cells.
  - Without obstacles the answer is C(2M, M); obstacles reduce it.

Methods:
  - solve: DP on MxM grid with obstacle detection.
  - is_blocked: Check the blocking condition for a point.
  - count_obstacles: Count total blocked points for analysis.

Complexity: O(M^2) for DP.
"""

from math import gcd, isqrt

# Helper: check if n is a perfect square
def is_perfect_square(n):
    """Check if n is a perfect square."""
    s = isqrt(n)
    return s * s == n

# Helper: check if (x, y) is blocked
def is_blocked(x, y):
    """A point is blocked if x+y is a perfect square and gcd(x,y) > 1."""
    return is_perfect_square(x + y) and gcd(x, y) > 1

def solve(M=50, MOD=10**9 + 7):
    """Count lattice paths from (0,0) to (M,M) avoiding blocked points."""
    blocked = set()
    for x in range(M + 1):
        for y in range(M + 1):
            if is_blocked(x, y):
                blocked.add((x, y))

    dp = [[0] * (M + 1) for _ in range(M + 1)]
    dp[0][0] = 0 if (0, 0) in blocked else 1
    for x in range(M + 1):
        for y in range(M + 1):
            if x == 0 and y == 0:
                continue
            if (x, y) in blocked:
                dp[x][y] = 0
                continue
            val = 0
            if x > 0:
                val += dp[x - 1][y]
            if y > 0:
                val += dp[x][y - 1]
            dp[x][y] = val % MOD
    return dp[M][M], blocked, dp

def count_obstacles(M):
    """Count and return blocked points in [0,M]x[0,M]."""
    blocked = []
    for x in range(M + 1):
        for y in range(M + 1):
            if is_blocked(x, y):
                blocked.append((x, y))
    return blocked

def unrestricted_paths(M):
    """C(2M, M) = number of lattice paths without obstacles."""
    from math import comb
    return comb(2 * M, M)

# Verification
# (0,0): x+y=0, perfect square yes, gcd(0,0) undefined but convention: not blocked
# since gcd(0,0) is undefined, dp[0][0] should be 1 (origin is not blocked)
assert not is_blocked(1, 0)  # 1+0=1 perfect sq, gcd(1,0)=1, not blocked
assert not is_blocked(0, 1)  # same
assert is_blocked(2, 2)      # 2+2=4 perfect sq, gcd(2,2)=2>1, blocked
assert not is_blocked(1, 3)  # 1+3=4 perfect sq, gcd(1,3)=1, not blocked

# Small grid check: M=2
# Blocked: (2,2) since 4 is perf sq and gcd(2,2)=2
# Paths: (0,0)->(1,0)->(2,0)->(2,1)->(2,2)=blocked, so dp[2][2]=0?
# Actually dp[2][2]=0 since it's blocked.
# Let's check M=3:
ans3, bl3, dp3 = solve(3)
assert (2, 2) in bl3

# Compute answer
ans, blocked, dp_full = solve()
print(f"Answer: {ans}")
