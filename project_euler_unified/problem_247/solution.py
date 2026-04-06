import heapq
import math

def solve():
    """
    Problem 247: Squares Under a Hyperbola
    Find the largest n such that the index of S_n is (3,3).
    Region: 1 <= x, 0 <= y <= 1/x.
    Greedily place largest squares. Index (left, below) counts squares
    to the left and below.
    """
    def calc_side(x0, y0):
        diff = x0 - y0
        return (math.sqrt(diff * diff + 4.0) - x0 - y0) / 2.0

    # Max-heap via negation: (-side, x0, y0, left, below)
    s0 = calc_side(1.0, 0.0)
    heap = [(-s0, 1.0, 0.0, 0, 0)]

    target_left, target_below = 3, 3
    n = 0
    last_n = -1
    viable = 1  # count of entries in heap with left<=3 and below<=3

    while viable > 0 and heap:
        neg_s, x0, y0, left, below = heapq.heappop(heap)
        s = -neg_s
        n += 1

        if left <= target_left and below <= target_below:
            viable -= 1

        if left == target_left and below == target_below:
            last_n = n

        # Right child
        rx, ry = x0 + s, y0
        rs = calc_side(rx, ry)
        if rs > 1e-15:
            heapq.heappush(heap, (-rs, rx, ry, left + 1, below))
            if left + 1 <= target_left and below <= target_below:
                viable += 1

        # Top child
        tx, ty = x0, y0 + s
        ts = calc_side(tx, ty)
        if ts > 1e-15:
            heapq.heappush(heap, (-ts, tx, ty, left, below + 1))
            if left <= target_left and below + 1 <= target_below:
                viable += 1

    print(last_n)

solve()
