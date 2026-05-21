"""
Problem 222: Sphere Packing

The tube length for an ordering r_1, ..., r_n is

    r_1 + r_n + sum gap(r_i, r_{i+1}),

where gap(x, y) = 2 * sqrt(R * (x + y - R)) and R = 50.

Because gap depends only on x + y and is concave in that sum, an exchange
argument collapses the search to the two pendulum orders:

    50, 48, 46, ..., 30, 31, 33, ..., 49
    49, 47, 45, ..., 31, 30, 32, ..., 50

Evaluating both is enough.
"""

import math


def gap(left, right, radius=50.0):
    return 2.0 * math.sqrt(radius * (left + right - radius))


def tube_length(order):
    total = order[0] + order[-1]
    for index in range(len(order) - 1):
        total += gap(order[index], order[index + 1])
    return total


def pendulum_orders(radii):
    low_to_high = sorted(radii)
    high_parity = [value for value in reversed(low_to_high) if value % 2 == low_to_high[-1] % 2]
    low_parity = [value for value in low_to_high if value % 2 != low_to_high[-1] % 2]

    yield high_parity + low_parity

    high_parity = [value for value in reversed(low_to_high) if value % 2 != low_to_high[-1] % 2]
    low_parity = [value for value in low_to_high if value % 2 == low_to_high[-1] % 2]
    yield high_parity + low_parity


def solve():
    radii = list(range(30, 51))
    best = min(tube_length(order) for order in pendulum_orders(radii))
    print(round(best * 1000))


if __name__ == "__main__":
    solve()
