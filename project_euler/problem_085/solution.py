"""
Problem 85: Counting Rectangles

Find the area of the rectangular grid whose rectangle count is
nearest to 2,000,000.

R(m, n) = C(m+1,2) * C(n+1,2) = m(m+1)/2 * n(n+1)/2

Answer: 2772
"""

import math


def solve():
    target = 2_000_000
    best_diff = float('inf')
    best_area = 0

    m = 1
    while m * (m + 1) // 2 <= target:
        tm = m * (m + 1) // 2
        val = 2 * target / tm
        n = int((-1 + math.sqrt(1 + 4 * val)) / 2)

        for nn in range(max(1, n - 1), n + 3):
            rects = tm * nn * (nn + 1) // 2
            diff = abs(rects - target)
            if diff < best_diff:
                best_diff = diff
                best_area = m * nn
        m += 1

    return best_area


def main():
    answer = solve()
    assert answer == 2772
    print(answer)


if __name__ == "__main__":
    main()
