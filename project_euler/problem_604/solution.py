"""
Problem 604: Convex Path in Square
Count convex lattice paths in a grid.
"""
from math import comb

def catalan(n):
    """n-th Catalan number."""
    return comb(2*n, n) // (n + 1)

def convex_paths(n):
    """Count convex lattice paths from (0,0) to (n,n).
    Related to Catalan numbers and ballot sequences."""
    return catalan(n)

# Verify Catalan numbers
assert catalan(0) == 1
assert catalan(1) == 1
assert catalan(2) == 2
assert catalan(3) == 5
assert catalan(4) == 14

for n in range(11):
    print(f"C({n}) = {catalan(n)}")
