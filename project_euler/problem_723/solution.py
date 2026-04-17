"""
Problem 723: Pythagorean Quadrilaterals

Count pythagorean lattice grid quadrilaterals inscribed in circles.
S(n) = sum_{d|n} f(sqrt(d)) where f(r) counts distinct pythagorean
lattice quadrilaterals with circumradius r.
"""

def r2(n):
    """Count representations of n as sum of two squares (order matters, signs matter)."""
    count = 0
    for x in range(int(n**0.5) + 1):
        y2 = n - x*x
        if y2 < 0:
            break
        y = int(y2**0.5)
        if y*y == y2:
            if x == 0 or y == 0:
                count += 2 if (x == 0) != (y == 0) else 1
                count += 2 if (x == 0) != (y == 0) else 1
            else:
                count += 4
            if x == y and y*y == y2:
                pass  # already counted
    # Simpler: direct enumeration
    count = 0
    for x in range(-int(n**0.5)-1, int(n**0.5)+2):
        for y in range(-int(n**0.5)-1, int(n**0.5)+2):
            if x*x + y*y == n:
                count += 1
    return count

def lattice_points(d):
    """Return list of (x,y) with x^2 + y^2 = d."""
    pts = []
    s = int(d**0.5) + 1
    for x in range(-s, s+1):
        y2 = d - x*x
        if y2 < 0:
            continue
        y = int(y2**0.5 + 0.5)
        if y*y == y2:
            if y > 0:
                pts.append((x, y))
                pts.append((x, -y))
            elif y == 0:
                pts.append((x, 0))
    return pts

def f_brute(d):
    """Brute force count of pythagorean lattice grid quadrilaterals for radius sqrt(d)."""
    pts = lattice_points(d)
    n = len(pts)
    count = 0
    # Check all ordered 4-tuples (with cyclic/reflection equivalence)
    for i in range(n):
        for j in range(n):
            if j == i: continue
            for k in range(n):
                if k in (i, j): continue
                for l in range(n):
                    if l in (i, j, k): continue
                    p1, p2, p3, p4 = pts[i], pts[j], pts[k], pts[l]
                    a2 = (p1[0]-p2[0])**2 + (p1[1]-p2[1])**2
                    b2 = (p2[0]-p3[0])**2 + (p2[1]-p3[1])**2
                    c2 = (p3[0]-p4[0])**2 + (p3[1]-p4[1])**2
                    d2 = (p4[0]-p1[0])**2 + (p4[1]-p1[1])**2
                    if a2 + b2 + c2 + d2 == 8*d:
                        count += 1
    # Each quadrilateral counted 8 times (4 rotations x 2 reflections)
    # But degenerate cases need care
    return count // 8

# Verify small cases
print(f"f(1) = {f_brute(1)}")   # Expected: 1
print(f"f(2) = {f_brute(2)}")   # Expected: 1
print(f"f(5) = {f_brute(5)}")   # Expected: 38
