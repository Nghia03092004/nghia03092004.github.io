"""
Problem 884: Lattice Walks with Barriers
LGV lemma, reflection principle, Catalan numbers.
"""

import numpy as np
from math import comb

def catalan(n):
    """n-th Catalan number."""
    return comb(2 * n, n) // (n + 1)

def ballot(m, n):
    """Paths from (0,0) to (m,n) staying weakly below y=x."""
    if n > m:
        return 0
    return comb(m + n, m) - comb(m + n, m + 1)

def lgv_det(sources, dests):
    """Compute LGV determinant for non-intersecting lattice paths."""
    k = len(sources)
    M = np.zeros((k, k))
    for i in range(k):
        for j in range(k):
            dx = dests[j][0] - sources[i][0]
            dy = dests[j][1] - sources[i][1]
            if dx >= 0 and dy >= 0:
                M[i][j] = comb(dx + dy, dx)
            else:
                M[i][j] = 0
    return round(np.linalg.det(M))

# --- Verification ---
print("=== Catalan Numbers ===")
for n in range(11):
    c = catalan(n)
    b = ballot(n, n)
    print(f"  C_{n} = {c}, ballot({n},{n}) = {b}, Match: {'OK' if c == b else 'FAIL'}")
    assert c == b

print("\n=== Ballot Problem ===")
for m, n in [(3,3),(4,4),(5,5),(4,2),(5,3),(6,2)]:
    total = comb(m + n, m)
    below = ballot(m, n)
    print(f"  ({m},{n}): total={total}, below y=x: {below}")

print("\n=== LGV Examples ===")
# Two non-intersecting paths
sources = [(0, 0), (0, 1)]
dests = [(3, 2), (3, 3)]
result = lgv_det(sources, dests)
print(f"  Sources={sources}, Dests={dests}: {result} non-intersecting path pairs")

# Three paths
sources3 = [(0, 0), (0, 1), (0, 2)]
dests3 = [(4, 2), (4, 3), (4, 4)]
result3 = lgv_det(sources3, dests3)
print(f"  3 paths: {result3} non-intersecting triples")

answer = catalan(10)
print(f"\nAnswer: C_10 = {answer}")

# --- 4-Panel Visualization ---
