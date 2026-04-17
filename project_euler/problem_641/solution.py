"""
Project Euler Problem 641: A Flea on a Chess Board

The colour sequence on the torus Z/(2W) x Z/(2H) is periodic with period
L = lcm(2W/gcd(dx,2W), 2H/gcd(dy,2H)).  Count white/black cells in one
period, then scale to N+1 positions via division with remainder.
"""

from math import gcd


def lcm(a, b):
    return a * b // gcd(a, b)


def count_colours(W, H, dx, dy, N):
    Lx = 2 * W // gcd(dx, 2 * W)
    Ly = 2 * H // gcd(dy, 2 * H)
    L = lcm(Lx, Ly)

    # Enumerate one full period
    white_prefix = [0] * (L + 1)
    x, y = 0, 0
    for k in range(L):
        colour = (x // W + y // H) % 2
        white_prefix[k + 1] = white_prefix[k] + (1 - colour)
        x = (x + dx) % (2 * W)
        y = (y + dy) % (2 * H)

    w = white_prefix[L]
    full, rem = divmod(N + 1, L)
    white_total = full * w + white_prefix[rem]
    black_total = (N + 1) - white_total
    return white_total, black_total


# Verification with small cases
assert count_colours(2, 3, 1, 1, 0) == (1, 0)  # origin is white
print(count_colours(2, 3, 3, 4, 999))
print("Problem 641: A Flea on a Chess Board")
