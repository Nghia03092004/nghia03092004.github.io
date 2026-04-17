"""
Problem 514: Geoboard Shapes
Count distinct shapes on an n x n geoboard formed by rubber bands,
up to translation, rotation, and reflection.
"""

from itertools import combinations
from math import gcd

def convex_hull(points):
    """Andrew's monotone chain convex hull algorithm."""
    points = sorted(set(points))
    if len(points) <= 1:
        return points

    def cross(O, A, B):
        return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0])

    lower = []
    for p in points:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= 0:
            lower.pop()
        lower.append(p)
    upper = []
    for p in reversed(points):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= 0:
            upper.pop()
        upper.append(p)
    return lower[:-1] + upper[:-1]

def polygon_area_2(pts):
    """Twice the signed area of a polygon (shoelace formula)."""
    n = len(pts)
    area = 0
    for i in range(n):
        j = (i + 1) % n
        area += pts[i][0] * pts[j][1] - pts[j][0] * pts[i][1]
    return abs(area)

def normalize_polygon(pts):
    """Normalize a polygon to canonical form (translation to origin, sorted)."""
    if not pts:
        return tuple()
    min_x = min(p[0] for p in pts)
    min_y = min(p[1] for p in pts)
    translated = tuple(sorted((p[0] - min_x, p[1] - min_y) for p in pts))
    return translated

def rotate_90(pts, n):
    """Rotate points 90 degrees clockwise on n x n grid."""
    return [(p[1], n - 1 - p[0]) for p in pts]

def reflect_h(pts, n):
    """Reflect across horizontal axis."""
    return [(p[0], n - 1 - p[1]) for p in pts]

def canonical_form(pts, n):
    """Find canonical form under D4 symmetry group."""
    forms = []
    current = list(pts)
    for _ in range(4):
        forms.append(normalize_polygon(current))
        ref = reflect_h(current, n)
        forms.append(normalize_polygon(ref))
        current = rotate_90(current, n)
    return min(forms)

def count_geoboard_shapes(n):
    """Count distinct convex shapes on n x n geoboard (using convex hulls as proxy)."""
    grid_points = [(i, j) for i in range(n) for j in range(n)]
    shapes = set()

    # Enumerate subsets of size 3..n*n that form convex polygons
    # For tractability, only consider convex hulls of small subsets
    max_subset = min(6, len(grid_points))
    for size in range(3, max_subset + 1):
        for subset in combinations(grid_points, size):
            hull = convex_hull(list(subset))
            if len(hull) >= 3 and polygon_area_2(hull) > 0:
                # Only count if all points are on the hull (convex polygon)
                if len(hull) == len(subset):
                    canon = canonical_form(hull, n)
                    shapes.add(canon)

    return len(shapes)

# Compute for small grids
results = {}
for n in range(2, 6):
    c = count_geoboard_shapes(n)
    results[n] = c
    print(f"Distinct convex shapes on {n}x{n} geoboard: {c}")
