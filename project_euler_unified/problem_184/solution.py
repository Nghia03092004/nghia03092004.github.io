"""
Problem 184: Triangles Containing the Origin

Count triangles with integer-coordinate vertices on/inside circle of radius 105
that strictly contain the origin.

Method: complementary counting with angular sweep.
"""

def solve():
    R = 105
    R2 = R * R

    # Collect non-origin lattice points
    pts = []
    for x in range(-R, R + 1):
        for y in range(-R, R + 1):
            if x == 0 and y == 0:
                continue
            if x * x + y * y <= R2:
                pts.append((x, y))

    N = len(pts)

    # Sort by angle using half-plane + cross product
    def sort_key(p):
        x, y = p
        # half: 0 for upper (y>0 or y==0,x>0), 1 for lower
        if y != 0:
            h = 0 if y > 0 else 1
        else:
            h = 0 if x > 0 else 1
        return h

    # Custom sort
    from functools import cmp_to_key

    def cmp(a, b):
        ha = 0 if (a[1] > 0 or (a[1] == 0 and a[0] > 0)) else 1
        hb = 0 if (b[1] > 0 or (b[1] == 0 and b[0] > 0)) else 1
        if ha != hb:
            return -1 if ha < hb else 1
        cross = a[0] * b[1] - a[1] * b[0]
        if cross > 0:
            return -1
        elif cross < 0:
            return 1
        return 0

    pts.sort(key=cmp_to_key(cmp))

    # Two-pointer sweep
    bad = 0
    j = 0
    for i in range(N):
        if j <= i:
            j = i + 1
        while j < i + N:
            jj = j % N
            cross = pts[i][0] * pts[jj][1] - pts[i][1] * pts[jj][0]
            if cross <= 0:
                break
            j += 1
        cnt = j - i - 1
        bad += cnt * (cnt - 1) // 2

    total = N * (N - 1) * (N - 2) // 6
    print(total - bad)

if __name__ == "__main__":
    solve()
