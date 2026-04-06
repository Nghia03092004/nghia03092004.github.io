"""
Problem 956: Random Matrix Determinant

Sum |det(A)| over all 3x3 matrices A with entries in {-1, 0, 1}.
Total: 3^9 = 19683 matrices.

Complexity: O(3^9) = O(19683), effectively O(1).
"""

from itertools import product

def det3x3(m):
    """Compute determinant of 3x3 matrix given as flat list of 9 elements."""
    a, b, c, d, e, f, g, h, i = m
    return a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g)

def solve():
    """Sum |det(A)| over all 3x3 matrices with entries in {-1, 0, 1}."""
    total = 0
    det_counts = {}
    for entries in product([-1, 0, 1], repeat=9):
        d = det3x3(entries)
        total += abs(d)
        det_counts[d] = det_counts.get(d, 0) + 1
    return total, det_counts

# --- Compute answer ---
answer, det_counts = solve()

# Verify basic properties
assert sum(det_counts.values()) == 3**9 == 19683
# Symmetry check: count(d) == count(-d)
for d in det_counts:
    if d != 0:
        assert det_counts[d] == det_counts.get(-d, 0), f"Asymmetry at d={d}"

print(answer)
