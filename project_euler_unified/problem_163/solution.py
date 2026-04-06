"""
Project Euler Problem 163: Cross-hatched Triangles

Count triangles in a cross-hatched equilateral triangle of size n = 36.

Six line families in oblique coordinates (u, v) with triangle
u >= 0, v >= 0, u + v <= n.  A triangle is formed by choosing one line
from each of three distinct families; we check containment and
non-concurrency via exact rational arithmetic.
"""


def solve():
    n = 36

    families = [(1, 0), (0, 1), (1, 1), (1, -1), (1, 2), (2, 1)]
    line_ranges = [
        list(range(0, n + 1)),
        list(range(0, n + 1)),
        list(range(0, n + 1)),
        list(range(-(n - 1), n)),
        list(range(1, 2 * n)),
        list(range(1, 2 * n)),
    ]

    def intersect(a1, b1, c1, a2, b2, c2):
        det = a1 * b2 - a2 * b1
        if det == 0:
            return None
        nu = c1 * b2 - c2 * b1
        nv = a1 * c2 - a2 * c1
        d = det
        if d < 0:
            nu, nv, d = -nu, -nv, -d
        return (nu, nv, d)

    def inside(nu, nv, d):
        return nu >= 0 and nv >= 0 and nu + nv <= n * d

    total = 0
    for f1 in range(6):
        a1, b1 = families[f1]
        for f2 in range(f1 + 1, 6):
            a2, b2 = families[f2]
            for f3 in range(f2 + 1, 6):
                a3, b3 = families[f3]
                for c1 in line_ranges[f1]:
                    for c2 in line_ranges[f2]:
                        p12 = intersect(a1, b1, c1, a2, b2, c2)
                        if p12 is None or not inside(*p12):
                            continue
                        for c3 in line_ranges[f3]:
                            p13 = intersect(a1, b1, c1, a3, b3, c3)
                            if p13 is None or not inside(*p13):
                                continue
                            p23 = intersect(a2, b2, c2, a3, b3, c3)
                            if p23 is None or not inside(*p23):
                                continue
                            nu1, nv1, d1 = p12
                            nu2, nv2, d2 = p13
                            if nu1 * d2 == nu2 * d1 and nv1 * d2 == nv2 * d1:
                                continue
                            total += 1

    print(total)


solve()
