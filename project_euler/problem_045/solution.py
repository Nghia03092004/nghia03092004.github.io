import math

def is_pentagonal(v):
    """Check if v is a pentagonal number."""
    if v <= 0:
        return False
    disc = 1 + 24 * v
    s = math.isqrt(disc)
    if s * s != disc:
        return False
    return (1 + s) % 6 == 0

def solve():
    """Find the next number after 40755 that is triangular, pentagonal,
    and hexagonal.

    Key insight: every hexagonal number H_m = m(2m-1) equals T_{2m-1},
    so all hexagonal numbers are triangular. We only need to find the
    next hexagonal number that is also pentagonal.
    """
    m = 144  # Start after H_143 = 40755
    while True:
        h = m * (2 * m - 1)
        if is_pentagonal(h):
            print(h)
            return
        m += 1

solve()
