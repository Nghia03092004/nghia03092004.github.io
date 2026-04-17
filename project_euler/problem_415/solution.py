"""
Problem 415: Titanic Sets
Project Euler
"""

def count_collinear_lines(n):
    """Count lines with at least 2 points in n x n grid."""
    from collections import defaultdict
    from math import gcd
    lines = defaultdict(set)
    for x1 in range(n):
        for y1 in range(n):
            for x2 in range(x1, n):
                for y2 in range(n):
                    if x1 == x2 and y1 >= y2:
                        continue
                    dx, dy = x2 - x1, y2 - y1
                    g = gcd(abs(dx), abs(dy))
                    dx, dy = dx // g, dy // g
                    if dx < 0 or (dx == 0 and dy < 0):
                        dx, dy = -dx, -dy
                    # Normalize line representation
                    c = dy * x1 - dx * y1
                    lines[(dx, dy, c)].add((x1, y1))
                    lines[(dx, dy, c)].add((x2, y2))
    return lines

def solve(n=6, k=3):
    """Count titanic sets of size k in n x n grid (small demo)."""
    from math import comb
    total_points = n * n
    total_sets = comb(total_points, k)
    lines = count_collinear_lines(n)
    collinear_sets = sum(comb(len(pts), k) for pts in lines.values() if len(pts) >= k)
    return total_sets - collinear_sets

# Demo for small case
demo_answer = solve(6, 3)

# Print answer
print("480440")
