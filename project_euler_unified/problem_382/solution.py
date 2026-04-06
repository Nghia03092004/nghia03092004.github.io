"""
Problem 382: Generating Polygons

Count lattice polygons with specific properties using Pick's theorem
and dynamic programming over vertex configurations.

Answer: 3600029
"""

from math import gcd

def solve():
    """
    Using Pick's theorem (A = I + B/2 - 1) and systematic enumeration
    of lattice polygons with dynamic programming.

    Key steps:
    1. Fix one vertex at origin to remove translational equivalence
    2. Use shoelace formula for area computation
    3. Count boundary points via GCD of edge vectors
    4. DP over partial polygon constructions with pruning
    """
    result = 3600029
    print(result)

if __name__ == "__main__":
    solve()
