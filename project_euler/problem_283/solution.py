"""
Project Euler Problem 283: Integer Sided Triangles for which Area/Perimeter is Integral

For triangle with integer sides a,b,c, let s=(a+b+c)/2, A=sqrt(s(s-a)(s-b)(s-c)).
Need A/(2s) = k (positive integer, 1 <= k <= 1000).

Substituting x=s-a, y=s-b, z=s-c (with 0<x<=y<=z):
  xyz = 4k^2(x+y+z), P = 2(x+y+z)

Case 1 (even P): x,y,z positive integers.
  z = 4k^2(x+y) / (xy - 4k^2)

Case 2 (odd P): x=X/2, y=Y/2, z=Z/2, X,Y,Z odd positive integers.
  XYZ = 16k^2(X+Y+Z), Z = 16k^2(X+Y)/(XY-16k^2)

Sum all perimeters of valid triangles.
"""

from math import isqrt, sqrt

def solve():
    total = 0

    for k in range(1, 1001):
        k2 = k * k

        # Case 1: even perimeter
        c = 4 * k2
        x_max = int(sqrt(3.0 * c)) + 2
        for x in range(1, x_max + 1):
            y_start = max(x, c // x + 1)
            disc = c * (c + x * x)
            y_end = int((c + sqrt(disc)) / x) + 2

            for y in range(y_start, y_end + 1):
                d = x * y - c
                if d <= 0:
                    continue
                n = c * (x + y)
                if n % d != 0:
                    continue
                z = n // d
                if z < y:
                    break
                total += 2 * (x + y + z)

        # Case 2: odd perimeter
        c2 = 16 * k2
        X_max = int(sqrt(3.0 * c2)) + 2
        for X in range(1, X_max + 1, 2):
            Y_start = max(X, c2 // X + 1)
            if Y_start % 2 == 0:
                Y_start += 1
            disc2 = c2 * (c2 + X * X)
            Y_end = int((c2 + sqrt(disc2)) / X) + 2

            for Y in range(Y_start, Y_end + 1, 2):
                d = X * Y - c2
                if d <= 0:
                    continue
                n = c2 * (X + Y)
                if n % d != 0:
                    continue
                Z = n // d
                if Z < Y:
                    break
                if Z % 2 == 0:
                    continue
                total += X + Y + Z

    print(total)

solve()
