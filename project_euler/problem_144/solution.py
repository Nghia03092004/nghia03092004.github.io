"""
Problem 144: Investigating Multiple Reflections of a Laser Beam

Simulate laser reflections inside the ellipse 4x^2 + y^2 = 100.
Entry at (0, 10.1), first hit at (1.4, -9.6).
Exit when |x| <= 0.01 at the top (y > 0).
"""

def solve():
    x0, y0 = 0.0, 10.1
    x1, y1 = 1.4, -9.6

    count = 0
    points = [(x0, y0), (x1, y1)]

    while True:
        # Incoming direction
        dx = x1 - x0
        dy = y1 - y0

        # Normal at (x1, y1) on ellipse 4x^2 + y^2 = 100
        nx = 4.0 * x1
        ny = y1

        # Reflect: d' = d - 2(d.n)/(n.n) * n
        dn = dx * nx + dy * ny
        nn = nx * nx + ny * ny
        rx = dx - 2.0 * dn / nn * nx
        ry = dy - 2.0 * dn / nn * ny

        # Next intersection parameter
        denom = 4.0 * rx * rx + ry * ry
        t = -2.0 * (4.0 * x1 * rx + y1 * ry) / denom

        x2 = x1 + t * rx
        y2 = y1 + t * ry

        count += 1
        points.append((x2, y2))

        # Exit check
        if abs(x2) <= 0.01 and y2 > 0:
            break

        x0, y0 = x1, y1
        x1, y1 = x2, y2

    return count


if __name__ == "__main__":
    answer = solve()
    assert answer == 354
    print(answer)
