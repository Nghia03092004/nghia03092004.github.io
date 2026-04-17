"""
Problem 920: Stern-Brocot Mediant

Find the depth of the fraction 355/113 in the Stern-Brocot tree.

The Stern-Brocot tree is a binary tree containing every positive rational
number exactly once, in lowest terms. Starting from the mediants of 0/1
and 1/0, each node is the mediant (a+c)/(b+d) of its left ancestor a/b
and right ancestor c/d.

Key results:
    - Depth of p/q equals the sum of partial quotients of its continued fraction
    - 355/113 = [3; 7, 16], so depth = 3 + 7 + 16 = 26
    - The path in the tree corresponds to the CF expansion:
      3 rights, 7 lefts, 16 rights (or vice versa depending on convention)
    - 355/113 is a famous approximation to pi (accurate to 6 decimal places)

Methods:
    1. Direct tree traversal (binary search in Stern-Brocot tree)
    2. Continued fraction expansion (depth = sum of partial quotients)
    3. Verification via mediant reconstruction
"""

from math import gcd

def sb_depth(p, q):
    """
    Find depth of p/q in the Stern-Brocot tree by direct traversal.
    Returns the 1-indexed depth (root mediants are at depth 1).
    """
    depth = 0
    la, lb = 0, 1   # left boundary: 0/1
    ra, rb = 1, 0   # right boundary: 1/0
    while True:
        ma, mb = la + ra, lb + rb  # mediant
        if ma == p and mb == q:
            return depth + 1
        if p * mb < ma * q:  # p/q < ma/mb -> go left
            ra, rb = ma, mb
        else:                # p/q > ma/mb -> go right
            la, lb = ma, mb
        depth += 1

def continued_fraction(p, q):
    """Return the continued fraction expansion [a0; a1, a2, ...] of p/q."""
    result = []
    while q > 0:
        a, r = divmod(p, q)
        result.append(a)
        p, q = q, r
    return result

def sb_depth_via_cf(p, q):
    """Depth in the Stern-Brocot tree = sum of CF partial quotients."""
    return sum(continued_fraction(p, q))

def sb_path_from_cf(p, q):
    """
    Return the path (sequence of 'R' and 'L') to reach p/q in the tree.
    CF [a0; a1, a2, ...] gives: a0 R's, a1 L's, a2 R's, ...
    """
    cf = continued_fraction(p, q)
    path = []
    directions = ['R', 'L']  # alternating
    for i, a in enumerate(cf):
        # Last partial quotient: use (a-1) moves then arrive
        if i == len(cf) - 1:
            path.extend([directions[i % 2]] * (a - 1))
        else:
            path.extend([directions[i % 2]] * a)
    return path

def verify_path(p, q):
    """Follow the path and verify we arrive at p/q."""
    path = sb_path_from_cf(p, q)
    la, lb = 0, 1
    ra, rb = 1, 0
    for step in path:
        ma, mb = la + ra, lb + rb
        if step == 'L':
            ra, rb = ma, mb
        else:
            la, lb = ma, mb
    # Final mediant should be p/q
    ma, mb = la + ra, lb + rb
    return ma == p and mb == q

# Verification
# 355/113 = [3; 7, 16], depth = 3+7+16 = 26
cf_355_113 = continued_fraction(355, 113)
assert cf_355_113 == [3, 7, 16], f"CF mismatch: {cf_355_113}"
assert sb_depth(355, 113) == 26
assert sb_depth_via_cf(355, 113) == 26
assert verify_path(355, 113)

# Simple cases
assert sb_depth(1, 1) == 1   # 1/1 is the root
assert sb_depth(1, 2) == 2   # 1/2 is left child of root
assert sb_depth(2, 1) == 2   # 2/1 is right child of root
assert continued_fraction(1, 2) == [0, 2]

# Cross-check methods on many fractions
for p in range(1, 30):
    for q in range(1, 30):
        if gcd(p, q) == 1:
            assert sb_depth(p, q) == sb_depth_via_cf(p, q), f"Mismatch at {p}/{q}"

# Solve
answer = sb_depth(355, 113)
print(answer)
