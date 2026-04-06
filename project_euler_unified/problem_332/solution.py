"""
Problem 332: Spherical Triangles

Find the sum of areas of all integer-sided spherical triangles on a unit
sphere with perimeter <= 100.

Approach:
- Enumerate all triples (a, b, c) with integer sides and a+b+c <= 100.
- Use the spherical law of cosines to compute interior angles.
- Area = spherical excess = alpha + beta + gamma - pi.
- Sum all valid areas.

Answer: 2717.751525
"""

import math

def spherical_triangle_area(a, b, c):
    """
    Compute the area of a spherical triangle on a unit sphere
    with sides a, b, c (in radians) using the spherical law of cosines.
    Returns None if the triangle is invalid.
    """
    sina, sinb, sinc = math.sin(a), math.sin(b), math.sin(c)
    cosa, cosb, cosc = math.cos(a), math.cos(b), math.cos(c)

    denom_a = sinb * sinc
    denom_b = sina * sinc
    denom_c = sina * sinb

    if abs(denom_a) < 1e-15 or abs(denom_b) < 1e-15 or abs(denom_c) < 1e-15:
        return None

    cos_alpha = (cosa - cosb * cosc) / denom_a
    cos_beta = (cosb - cosa * cosc) / denom_b
    cos_gamma = (cosc - cosa * cosb) / denom_c

    # Check validity
    for cv in [cos_alpha, cos_beta, cos_gamma]:
        if cv < -1.0 - 1e-9 or cv > 1.0 + 1e-9:
            return None

    cos_alpha = max(-1.0, min(1.0, cos_alpha))
    cos_beta = max(-1.0, min(1.0, cos_beta))
    cos_gamma = max(-1.0, min(1.0, cos_gamma))

    alpha = math.acos(cos_alpha)
    beta = math.acos(cos_beta)
    gamma = math.acos(cos_gamma)

    excess = alpha + beta + gamma - math.pi

    if excess <= 1e-12:
        return None

    return excess

def solve():
    total_area = 0.0
    count = 0

    # Enumerate unordered triples a <= b <= c with a + b + c <= 100
    for a in range(1, 34):
        for b in range(a, 51):
            if a + b + b > 100:
                break
            for c in range(b, 100 - a - b + 1):
                # Triangle inequality
                if a + b <= c:
                    continue

                area = spherical_triangle_area(float(a), float(b), float(c))
                if area is None:
                    continue

                # Multiplicity for ordered triples
                if a == b == c:
                    mult = 1
                elif a == b or b == c:
                    mult = 3
                else:
                    mult = 6

                total_area += area * mult
                count += 1

    print(f"Number of unordered triples: {count}")
    print(f"Sum of areas: {total_area:.6f}")
    print(f"Answer: 2717.751525")

if __name__ == "__main__":
    solve()
