"""
Problem 222: Sphere Packing

Find the minimum length tube (radius 50mm) to contain 21 spheres of radii 30-50mm.

Key insight: spheres alternate sides in the tube. The vertical gap between
consecutive spheres i,j (on opposite sides) is 2*sqrt(R*(ri+rj-R)).

We use bitmask DP to find the optimal ordering.
Note: 2^21 * 21 ~ 44M states, feasible in C++ but slow in Python.
For Python, we use a greedy/heuristic approach validated against the known answer,
or we optimize the DP.

Actually, with 21 spheres the DP has 2^21 = 2M masks * 21 = ~44M states.
In Python this is too slow with dicts, so we use a known mathematical insight:
the optimal arrangement alternates large and small spheres, and we can verify
with a reduced search.

The optimal ordering can be found by noting that we should interleave
small and large radii. Specifically, arrange as:
30, 50, 31, 49, 32, 48, ..., 39, 41, 40
But one sphere (the smallest) might be better dropped to the end.

We'll try all arrangements of the "interleaved" type and pick the best.
"""

import math
from itertools import permutations

def tube_length(order, R=50.0):
    """Compute tube length for a given ordering of sphere radii."""
    total = order[0] + order[-1]  # first and last sphere touch the ends
    for i in range(len(order) - 1):
        s = order[i] + order[i+1]
        total += 2 * math.sqrt(R * (s - R))
    return total

def solve():
    R = 50.0
    radii = list(range(30, 51))  # 30 to 50, 21 spheres
    N = len(radii)

    # Optimal strategy: interleave small and large.
    # Try different interleaving patterns.
    # Pattern: pick from both ends alternately.

    best = float('inf')
    best_order = None

    # Try all 2^21 isn't feasible in Python, but we can try systematic interleavings.
    # The key insight: split into two groups and interleave.
    # Group A (small): 30-39 (10 spheres), Group B (large): 40-50 (11 spheres)
    # Interleave: B, A, B, A, ..., B

    # Actually, let's just try: for each possible "excluded from alternation" sphere,
    # try placing it at each position.

    # Better: the known optimal for this problem uses a specific interleaving.
    # Let's try the approach: split into odds and evens by index.

    # Approach: try all ways to split 21 spheres into two groups and interleave.
    # The DP approach with bitmask is the correct one. Let's implement it
    # efficiently using numpy-style or just accept it'll be slow.

    # For Python, let's use a simplified bitmask DP with pruning.
    # Actually, with careful implementation using arrays, 44M states might work.

    # Alternative fast approach for Python: since we know the answer,
    # let's use the greedy nearest-neighbor heuristic with verification.

    # Actually the cleanest Python approach: use the known structure.
    # The optimal arrangement places spheres in a zigzag pattern.
    # All spheres with r_i + r_j > R (always true here) go on alternating sides.
    # We need to minimize sum of 2*sqrt(R*(r_i + r_{i+1} - R)).
    # This is a TSP-like problem. For 21 cities, we can solve it with DP.

    # Let's implement bitmask DP in Python, but with bit operations.
    # 2^21 = 2097152, times 21 = ~44M. We'll use a flat array.

    FULL = 1 << N
    INF = float('inf')

    # dp[mask * N + last] = min cost
    dp = [INF] * (FULL * N)

    # Precompute dz
    dz = [[0.0]*N for _ in range(N)]
    for i in range(N):
        for j in range(N):
            s = radii[i] + radii[j]
            dz[i][j] = 2.0 * math.sqrt(R * (s - R))

    # Initialize
    for i in range(N):
        dp[(1 << i) * N + i] = radii[i]

    for mask in range(1, FULL):
        for j in range(N):
            if not (mask & (1 << j)):
                continue
            val = dp[mask * N + j]
            if val >= INF:
                continue
            for k in range(N):
                if mask & (1 << k):
                    continue
                nmask = mask | (1 << k)
                nval = val + dz[j][k]
                idx = nmask * N + k
                if nval < dp[idx]:
                    dp[idx] = nval

    best = INF
    full_base = (FULL - 1) * N
    for j in range(N):
        val = dp[full_base + j] + radii[j]
        if val < best:
            best = val

    ans = round(best * 1000)
    print(ans)

if __name__ == "__main__":
    solve()
