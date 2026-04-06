"""
Problem 142: Perfect Square Collection

Find the smallest x+y+z with x>y>z>0 such that
x+y, x-y, x+z, x-z, y+z, y-z are all perfect squares.

Let a^2=x+y, b^2=x-y, c^2=x+z, d^2=x-z, e^2=y+z, f^2=y-z.

Key relations:
  a^2 - f^2 = c^2  =>  a^2 = c^2 + f^2
  c^2 - b^2 = e^2  =>  c^2 = b^2 + e^2
  (and then d^2 = a^2 - e^2, verify b^2 + f^2 = d^2)

Strategy: for each a, find all ways to write a^2 = c^2 + f^2.
For each such (c, f), check if c^2 - b^2 = e^2 for some b, e
with a^2 - e^2 = d^2 (perfect square) and b^2 + f^2 = d^2.

Simpler: for each a, find all (c, f) with a^2 = c^2 + f^2.
Then for each (c, f), iterate over e < c, check if c^2 - e^2 = b^2
(perfect square), and then check if a^2 - e^2 = d^2 (perfect square).
"""

from math import isqrt

def solve():
    best = float('inf')
    best_xyz = None

    for a in range(3, 1001):
        a2 = a * a
        # Find all (c, f) with a^2 = c^2 + f^2, c > 0, f > 0
        for f in range(1, a):
            c2 = a2 - f * f
            if c2 <= 0:
                break
            c = isqrt(c2)
            if c * c != c2:
                continue
            if c <= 0:
                continue

            # Now find e such that c^2 - e^2 = b^2 (perfect square)
            # e must satisfy: e > 0, c^2 - e^2 > 0 => e < c
            # Also: a^2 - e^2 must be a perfect square d^2
            # And: b^2 + f^2 = d^2
            for e in range(1, c):
                b2 = c * c - e * e
                if b2 <= 0:
                    continue
                b = isqrt(b2)
                if b * b != b2:
                    continue

                # Check a^2 - e^2 = d^2
                d2 = a2 - e * e
                if d2 <= 0:
                    continue
                d = isqrt(d2)
                if d * d != d2:
                    continue

                # Verify b^2 + f^2 = d^2
                if b2 + f * f != d2:
                    continue

                # Compute x, y, z
                # x = (a^2 + b^2) / 2, y = (a^2 - b^2) / 2
                if (a2 + b2) % 2 != 0:
                    continue

                x = (a2 + b2) // 2
                y = (a2 - b2) // 2

                # z from e^2 = y+z => z = e^2 - y
                z = e * e - y

                if x > y > z > 0:
                    s = x + y + z
                    if s < best:
                        best = s
                        best_xyz = (x, y, z)

    print(best)

    if best_xyz:
        x, y, z = best_xyz
        print(f"x={x}, y={y}, z={z}")
        vals = {'x+y': x+y, 'x-y': x-y, 'x+z': x+z, 'x-z': x-z, 'y+z': y+z, 'y-z': y-z}
        for name, val in vals.items():
            s = isqrt(val)
            print(f"  {name} = {val} = {s}^2 (check: {s*s == val})")

if __name__ == '__main__':
    solve()
