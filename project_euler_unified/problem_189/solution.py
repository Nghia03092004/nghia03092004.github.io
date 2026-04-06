"""
Problem 189: Tri-colouring a Triangular Grid

Count valid 3-colourings of a triangular grid of side 8 where no two
adjacent triangles share the same colour.
"""
from collections import defaultdict

def solve():
    N = 8

    # Row 1: 1 upward triangle
    # State = tuple of colors of upward triangles in the row
    dp = defaultdict(int)
    for c in range(3):
        dp[(c,)] += 1

    for row in range(2, N + 1):
        new_dp = defaultdict(int)

        for prev_up, cnt in dp.items():
            # Generate valid colorings of current row
            # Current row: row upward triangles, (row-1) downward triangles
            # Sequence: up[0], down[0], up[1], down[1], ..., down[row-2], up[row-1]

            cur_up = [0] * row
            cur_down = [0] * (row - 1)

            def generate(pos):
                if pos == 2 * row - 1:
                    new_dp[tuple(cur_up)] += cnt
                    return

                if pos % 2 == 0:
                    # Upward triangle at index pos//2
                    idx = pos // 2
                    for c in range(3):
                        if idx > 0 and c == cur_down[idx - 1]:
                            continue
                        cur_up[idx] = c
                        generate(pos + 1)
                else:
                    # Downward triangle at index pos//2
                    idx = pos // 2
                    for c in range(3):
                        if c == cur_up[idx]:
                            continue
                        if c == prev_up[idx]:
                            continue
                        cur_down[idx] = c
                        generate(pos + 1)

            generate(0)

        dp = new_dp

    total = sum(dp.values())
    print(total)

if __name__ == "__main__":
    solve()
