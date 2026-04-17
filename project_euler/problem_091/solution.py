"""
Problem 91: Right Triangles with Integer Coordinates

Count right triangles with vertices O=(0,0), P, Q on the integer grid
[0,N] x [0,N] by counting how many lattice points lie on the line through
each possible right-angle vertex and perpendicular to OP.
"""

from math import gcd


def count_right_triangles(n):
    total = 3 * n * n

    for x in range(1, n + 1):
        for y in range(1, n + 1):
            g = gcd(x, y)
            step_x = y // g
            step_y = x // g

            total += min((n - x) // step_x, y // step_y)
            total += min(x // step_x, (n - y) // step_y)

    return total


def solve():
    return count_right_triangles(50)


if __name__ == "__main__":
    answer = solve()
    assert answer == 14234
    print(answer)
