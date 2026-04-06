"""
Problem 932: Triangular Number Intersections

Count pairs (a, b) with 1 <= a < b <= limit where T_a + T_b is itself
a triangular number, i.e. T_a + T_b = T_c for some c >= 1.

Key identity:
  T_a + T_b = a(a+1)/2 + b(b+1)/2 = (a^2 + b^2 + a + b) / 2.
  This equals T_c = c(c+1)/2 iff a^2 + b^2 + a + b = c(c+1),
  i.e. 4(a^2 + b^2 + a + b) + 1 = (2c+1)^2, so we need
  2(2a+1)^2 + 2(2b+1)^2 - 1 to be a perfect square (of the form (2c+1)^2).

Methods:
  - solve: Build set of triangulars, iterate pairs, check membership.
  - is_triangular: Check if a number is triangular via discriminant.
  - count_pairs_formula: Use the algebraic identity to check via perfect square test.

Complexity: O(limit^2) for brute-force pair enumeration.
"""

from math import isqrt

# Helper: check if n is triangular
def is_triangular(n):
    """Check if n = k(k+1)/2 for some non-negative integer k."""
    if n < 0:
        return False
    # 8n + 1 must be a perfect square
    disc = 8 * n + 1
    s = isqrt(disc)
    return s * s == disc

def solve(limit):
    """Count pairs (a,b), 1<=a<b<=limit, where T_a + T_b is triangular."""
    tri_set = set()
    n = 1
    max_val = limit * (limit + 1)  # T_a + T_b <= T_limit + T_limit ~ limit^2
    while n * (n + 1) // 2 <= max_val:
        tri_set.add(n * (n + 1) // 2)
        n += 1
    count = 0
    pairs = []
    for a in range(1, limit + 1):
        ta = a * (a + 1) // 2
        for b in range(a + 1, limit + 1):
            tb = b * (b + 1) // 2
            if ta + tb in tri_set:
                count += 1
                if len(pairs) < 500:
                    pairs.append((a, b))
    return count, pairs

def count_pairs_formula(limit):
    """Count pairs using algebraic identity and perfect square test."""
    count = 0
    for a in range(1, limit + 1):
        for b in range(a + 1, limit + 1):
            val = a * a + b * b + a + b
            # val = c(c+1), so 4*val + 1 = (2c+1)^2
            disc = 4 * val + 1
            s = isqrt(disc)
            if s * s == disc:
                count += 1
    return count

# Verification
# T_1 + T_2 = 1 + 3 = 4 = not triangular (T_2=3, T_3=6)
assert not is_triangular(4)
# T_1 + T_5 = 1 + 15 = 16 = not triangular
assert not is_triangular(16)
# T_2 + T_3 = 3 + 6 = 9 = not triangular (T_3=6, T_4=10)
assert not is_triangular(9)

# Cross-check two methods for small limit
c1, _ = solve(50)
c2 = count_pairs_formula(50)
assert c1 == c2, f"Mismatch: {c1} vs {c2}"

# Compute answer (for visualization range)
count_small, pairs = solve(500)
print(f"Pairs found (limit=500): {count_small}")
