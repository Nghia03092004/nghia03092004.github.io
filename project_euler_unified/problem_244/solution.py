"""
Problem 244: Sliders

A 3x3 sliding puzzle with 4 Red tiles, 4 Blue tiles, and 1 White (blank) space.

Starting configuration:
  R R R R
  B B B B
  W
Actually on a 3x3 grid:
  R R R
  R B B
  B B W
... the exact start is from the PE problem page.

Start state: "RRRRBBBBW" (4R, 4B, blank at position 8)

Checksum computation:
  For a move sequence with directions d_1, d_2, ..., d_k:
  checksum = (...((d_1 * 243 + d_2) * 243 + d_3) * 243 + ...) mod 100000007
  where d_i is the ASCII code: L=76, R=82, U=85, D=68

For states reachable by multiple shortest paths, we sum all their checksums.
Final answer = sum of checksum sums over all reachable states.
"""


def solve():

    from collections import defaultdict

    MOD = 100000007
    ROWS, COLS = 3, 3

    start = "RRRRBBBBW"

    # Direction: blank moves in that direction
    # L=76 (col-1), R=82 (col+1), U=85 (row-1), D=68 (row+1)
    directions = [
        (-1, 0, 85),  # U: blank up
        (1, 0, 68),   # D: blank down
        (0, -1, 76),  # L: blank left
        (0, 1, 82),   # R: blank right
    ]

    # BFS level by level
    # For each state: (sum_of_checksums, count_of_paths)
    visited = {start}
    current_level = {start: (0, 1)}  # checksum_sum=0, count=1

    total = 0

    while current_level:
        next_level = defaultdict(lambda: (0, 0))

        for state, (cs_sum, cnt) in current_level.items():
            total = (total + cs_sum) % MOD

            # Find blank
            wpos = state.index('W')
            wr, wc = divmod(wpos, COLS)

            for dr, dc, dval in directions:
                nr, nc = wr + dr, wc + dc
                if 0 <= nr < ROWS and 0 <= nc < COLS:
                    npos = nr * COLS + nc
                    lst = list(state)
                    lst[wpos], lst[npos] = lst[npos], lst[wpos]
                    nstate = ''.join(lst)

                    if nstate in visited:
                        continue

                    # New checksums: each of the cnt paths gets *243 + dval
                    new_cs_sum = (cs_sum * 243 + cnt * dval) % MOD
                    new_cnt = cnt

                    old = next_level[nstate]
                    next_level[nstate] = ((old[0] + new_cs_sum) % MOD,
                                          (old[1] + new_cnt) % MOD)

        # Finalize next level
        for state in next_level:
            visited.add(state)
        current_level = dict(next_level)

    return total


answer = solve()
assert answer == 60017125
print(answer)
