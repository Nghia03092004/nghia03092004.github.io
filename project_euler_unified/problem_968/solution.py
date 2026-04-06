"""
Problem 968: Stern-Brocot Tree BFS

Sum of the first 1000 numerators in a BFS traversal of the Stern-Brocot tree.

The Stern-Brocot tree is an infinite binary tree of fractions where each
node a/b has left child a/(a+b) and right child (a+b)/b. Starting from
1/1, BFS yields all positive rationals in lowest terms exactly once.

Key results:
    - Root: 1/1
    - Left child of a/b: a/(a+b)
    - Right child of a/b: (a+b)/b
    - BFS order gives: 1/1, 1/2, 2/1, 1/3, 3/2, 2/3, 3/1, ...
    - Sum of first 1000 numerators

Methods:
    1. stern_brocot_bfs         — BFS traversal collecting fractions
    2. stern_brocot_level       — generate a specific level of the tree
    3. verify_coprimality       — check all fractions are in lowest terms
    4. fraction_density_analysis — analyze distribution of fractions
"""

import numpy as np
from collections import deque
from math import gcd

def stern_brocot_bfs(K):
    """Return first K fractions (a, b) from BFS of Stern-Brocot tree."""
    queue = deque([(1, 1)])
    fractions = []
    count = 0
    while count < K:
        a, b = queue.popleft()
        fractions.append((a, b))
        count += 1
        queue.append((a, a + b))
        queue.append((a + b, b))
    return fractions

def stern_brocot_level(level):
    """Generate all fractions at a given BFS level (0-indexed)."""
    if level == 0:
        return [(1, 1)]
    nodes = [(1, 1)]
    for _ in range(level):
        next_nodes = []
        for a, b in nodes:
            next_nodes.append((a, a + b))
            next_nodes.append((a + b, b))
        nodes = next_nodes
    return nodes

def verify_coprimality(fractions):
    """Check that all fractions a/b have gcd(a,b) = 1."""
    for a, b in fractions:
        assert gcd(a, b) == 1, f"gcd({a},{b}) = {gcd(a,b)} != 1"
    return True

def numerator_statistics(fractions):
    """Compute statistics of numerators."""
    nums = [a for a, b in fractions]
    return {
        'sum': sum(nums),
        'mean': np.mean(nums),
        'median': np.median(nums),
        'max': max(nums),
        'min': min(nums),
    }

# Verification
# First few BFS fractions should be: 1/1, 1/2, 2/1, 1/3, 3/2, 2/3, 3/1
first_7 = stern_brocot_bfs(7)
expected = [(1, 1), (1, 2), (2, 1), (1, 3), (3, 2), (2, 3), (3, 1)]
assert first_7 == expected, f"Expected {expected}, got {first_7}"

# Verify coprimality for first 100
fracs_100 = stern_brocot_bfs(100)
assert verify_coprimality(fracs_100)

# Level 0 has 1 node, level k has 2^k nodes
for lvl in range(5):
    assert len(stern_brocot_level(lvl)) == 2 ** lvl

# Compute answer
K = 1000
fractions = stern_brocot_bfs(K)
numerators = [a for a, b in fractions]
answer = sum(numerators)
print(answer)
