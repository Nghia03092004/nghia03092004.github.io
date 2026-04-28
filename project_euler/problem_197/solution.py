"""
Problem 197: Recursively Defined Sequence

The sequence converges quickly to a 2-cycle, so a modest fixed iteration count
is enough to recover the final 9-digit decimal answer.
"""

import math


def f(x):
    return math.floor(2 ** (30.403243784 - x * x)) * 1e-9


def solve_scaled(iterations=1000):
    u = -1.0
    for _ in range(iterations):
        u = f(u)
    v = f(u)
    return math.floor((u + v) * 1e9)


if __name__ == "__main__":
    scaled = solve_scaled()
    assert scaled == 1710637717
    print(f"{scaled / 1e9:.9f}")
