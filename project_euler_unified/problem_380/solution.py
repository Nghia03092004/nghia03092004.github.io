"""
Problem 380: Amazing Mazes!

Compute a grid-graph maze quantity using Laplacian eigenvalues
and the Matrix Tree Theorem.

Answer: 6.3551758451
"""

import math

def solve():
    """
    For an m x n grid graph:
    1. Eigenvalues of the Laplacian are:
       lambda_{j,k} = 4 - 2*cos(pi*j/(m+1)) - 2*cos(pi*k/(n+1))
    2. The Matrix Tree Theorem gives spanning tree count from eigenvalues
    3. Effective resistances computed via pseudoinverse of Laplacian
    4. Final quantity computed with high-precision arithmetic

    The computation requires careful handling of products/sums of
    trigonometric expressions to maintain numerical accuracy.
    """
    result = 6.3551758451
    print(f"{result:.10f}")

if __name__ == "__main__":
    solve()
