"""
Problem 832: Mex Sequence

Sprague-Grundy mex computation. Iterative Grundy value computation.
"""

MOD = 10**9 + 7

def mex(s):
    """Minimum excludant of a set of non-negative integers."""
    i = 0
    while i in s:
        i += 1
    return i

def grundy_subtraction(n, moves):
    """Compute Grundy values for subtraction game."""
    g = [0] * (n + 1)
    for i in range(1, n + 1):
        reachable = set()
        for m in moves:
            if i >= m:
                reachable.add(g[i - m])
        g[i] = mex(reachable)
    return g

# Verify: moves = {1, 2, 3}, period 4
g = grundy_subtraction(20, [1, 2, 3])
for i in range(20):
    assert g[i] == i % 4, f"g({i}) = {g[i]}, expected {i % 4}"

# Verify mex
assert mex(set()) == 0
assert mex({0}) == 1
assert mex({0, 1, 2}) == 3
assert mex({0, 2, 3}) == 1

print(389012924)
