import math


def pent(n):
    """Compute the n-th pentagonal number P_n = n(3n-1)/2."""
    return n * (3 * n - 1) // 2


def is_pentagonal(v):
    """Test if v is pentagonal: v = P_n iff (1 + sqrt(1+24v)) / 6 is a
    positive integer, equivalently 1+24v is a perfect square s^2 with
    s = 5 mod 6.
    """
    if v <= 0:
        return False
    disc = 1 + 24 * v
    s = math.isqrt(disc)
    if s * s != disc:
        return False
    return (1 + s) % 6 == 0


def solve():
    """Find the pair (P_j, P_k) with j < k such that both P_k - P_j and
    P_k + P_j are pentagonal, and D = P_k - P_j is minimised.

    Search strategy: iterate k, for each k try j = k-1 downto 1.
    Terminate outer loop when the minimum gap 3k-2 exceeds current best.
    """
    best = float('inf')
    for k in range(2, 10000):
        pk = pent(k)
        if 3 * k - 2 >= best:
            break
        for j in range(k - 1, 0, -1):
            pj = pent(j)
            diff = pk - pj
            if diff >= best:
                break
            if is_pentagonal(diff) and is_pentagonal(pk + pj):
                best = diff
    print(best)


solve()
