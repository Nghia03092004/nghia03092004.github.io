"""
Project Euler Problem 252: Convex Holes

Find the largest area convex hole among 500 LCG-generated points.
A convex hole is a convex polygon whose vertices are from the point set
and whose interior contains no other points from the set.

Algorithm: For each anchor point p0, sort remaining points by polar angle,
then DP on (last two vertices) tracking first vertex for closure checks.
Empty-triangle tests use precomputed bitsets.

Answer: 104924.0
"""

import math
from functools import cmp_to_key


def cross(O, A, B):
    return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0])


def solve():
    # Generate points
    s = 290797
    t = []
    for _ in range(1000):
        s = (s * s) % 50515093
        t.append(s % 2000 - 1000)
    pts = [(t[2 * k], t[2 * k + 1]) for k in range(500)]
    n = len(pts)

    WORDS = (n + 63) // 64

    # Precompute bitsets: B[i][j] = set of points strictly left of edge i->j
    B = [[[0] * WORDS for _ in range(n)] for _ in range(n)]
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            for p in range(n):
                if p == i or p == j:
                    continue
                if cross(pts[i], pts[j], pts[p]) > 0:
                    B[i][j][p // 64] |= 1 << (p % 64)

    def triangle_empty(a, b, c):
        for w in range(WORDS):
            if B[a][b][w] & B[b][c][w] & B[c][a][w]:
                return False
        return True

    best = 0

    for p0 in range(n):
        idx = [i for i in range(n) if i != p0]

        def cmp(a, b):
            ax = pts[a][0] - pts[p0][0]
            ay = pts[a][1] - pts[p0][1]
            bx = pts[b][0] - pts[p0][0]
            by_ = pts[b][1] - pts[p0][1]
            qa = 0 if (ay > 0 or (ay == 0 and ax > 0)) else 1
            qb = 0 if (by_ > 0 or (by_ == 0 and bx > 0)) else 1
            if qa != qb:
                return -1 if qa < qb else 1
            cr = ax * by_ - ay * bx
            if cr != 0:
                return 1 if cr > 0 else -1
            da = ax * ax + ay * ay
            db = bx * bx + by_ * by_
            if da != db:
                return -1 if da < db else 1
            return 0

        idx.sort(key=cmp_to_key(cmp))
        m = len(idx)
        dp = {}
        fv = {}

        # Base: triangles (p0, j, k)
        for j in range(m):
            for k in range(j + 1, m):
                cr = cross(pts[p0], pts[idx[j]], pts[idx[k]])
                if cr <= 0:
                    continue
                if not triangle_empty(p0, idx[j], idx[k]):
                    continue
                dp[(j, k)] = cr
                fv[(j, k)] = j
                best = max(best, cr)

        # Extend
        for k in range(m):
            for j in range(k):
                if (j, k) not in dp:
                    continue
                f = fv[(j, k)]
                area_jk = dp[(j, k)]
                for l in range(k + 1, m):
                    cr1 = cross(pts[idx[j]], pts[idx[k]], pts[idx[l]])
                    if cr1 <= 0:
                        continue
                    cr2 = cross(pts[p0], pts[idx[k]], pts[idx[l]])
                    if cr2 <= 0:
                        continue
                    if not triangle_empty(p0, idx[k], idx[l]):
                        continue
                    if cross(pts[p0], pts[idx[f]], pts[idx[l]]) <= 0:
                        continue
                    new_area = area_jk + cr2
                    if (k, l) not in dp or new_area > dp[(k, l)]:
                        dp[(k, l)] = new_area
                        fv[(k, l)] = f
                    if (cross(pts[idx[k]], pts[idx[l]], pts[p0]) > 0
                            and cross(pts[idx[l]], pts[p0], pts[idx[f]]) > 0):
                        best = max(best, new_area)

    print(f"{best / 2.0:.1f}")


if __name__ == "__main__":
    solve()
