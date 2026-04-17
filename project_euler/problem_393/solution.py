"""
Problem 393: Migrating Ants
An n x n grid of squares contains n^2 ants, one ant per square.
All ants simultaneously move to an adjacent square (up/down/left/right).
f(n) counts configurations where no two ants end on the same square
and no two ants cross the same edge. Given f(4) = 88, find f(10).
"""

import time

def f(n):
    """
    Compute f(n) using profile dynamic programming, processing the grid
    row by row and within each row column by column.

    State between rows: for each column j, encode the vertical flow:
      (d, u) where d=1 if an ant moves down, u=1 if an ant moves up.
      The no-crossing constraint forbids (d=1, u=1) at any column,
      leaving 3 valid states per column: (0,0), (0,1), (1,0).

    Within each row, process columns left to right tracking horizontal
    flow. The no-crossing constraint also forbids simultaneous left and
    right flow at any column boundary (horizontal swap).

    For each cell, exactly one ant arrives and one ant departs. We enforce:
      incoming = top_down + from_left + bottom_up + from_right = 1
      outgoing = top_up + to_left + bottom_down + to_right = 1
    """

    def process_row(n, top_state_int, is_last_row):
        """Given the vertical flow state above this row, compute all valid
        bottom-boundary states and their counts."""
        top = []
        ts = top_state_int
        for j in range(n):
            val = ts % 3
            if val == 0:
                top.append((0, 0))
            elif val == 1:
                top.append((0, 1))
            else:
                top.append((1, 0))
            ts //= 3

        # Column-by-column DP. State: (r_out, from_right, bot_int)
        # r_out: 1 if this cell sends its ant right
        # from_right: 1 if this cell declared incoming from right (demands
        #   the next column to send its ant left)
        dp = {}

        # Process column 0
        td, tu = top[0]
        S_in = 1 - td
        S_out = 1 - tu
        if 0 <= S_in <= 1 and 0 <= S_out <= 1:
            in_choices = [(0, 0)] if S_in == 0 else [(0, 1), (1, 0)]
            out_choices = [(0, 0)] if S_out == 0 else [(0, 1), (1, 0)]
            for bot_u, from_right in in_choices:
                for bot_d, r_out in out_choices:
                    if is_last_row and (bot_d or bot_u):
                        continue
                    if bot_d and bot_u:  # no vertical swap
                        continue
                    if r_out and from_right:  # no horizontal swap
                        continue
                    bv = 0 if (bot_d == 0 and bot_u == 0) else (1 if bot_u else 2)
                    key = (r_out, from_right, bv)
                    dp[key] = dp.get(key, 0) + 1

        # Process columns 1..n-1
        pow3 = [3 ** j for j in range(n)]
        for j in range(1, n):
            new_dp = {}
            td_j, tu_j = top[j]
            for (r_in_j, l_demand_j, bot_int), cnt in dp.items():
                if r_in_j and l_demand_j:  # horizontal swap forbidden
                    continue
                S_in = 1 - td_j - r_in_j
                S_out = 1 - tu_j - l_demand_j
                if S_in < 0 or S_in > 1 or S_out < 0 or S_out > 1:
                    continue
                in_choices = [(0, 0)] if S_in == 0 else [(0, 1), (1, 0)]
                out_choices = [(0, 0)] if S_out == 0 else [(0, 1), (1, 0)]
                for bot_u, from_right in in_choices:
                    for bot_d, r_out in out_choices:
                        if is_last_row and (bot_d or bot_u):
                            continue
                        if bot_d and bot_u:
                            continue
                        if j == n - 1 and (r_out or from_right):
                            continue
                        if r_out and from_right:
                            continue
                        bv = 0 if (bot_d == 0 and bot_u == 0) else (1 if bot_u else 2)
                        new_bot = bot_int + bv * pow3[j]
                        key = (r_out, from_right, new_bot)
                        new_dp[key] = new_dp.get(key, 0) + cnt
            dp = new_dp

        result = {}
        for (_, _, bot_int), cnt in dp.items():
            result[bot_int] = result.get(bot_int, 0) + cnt
        return result

    zero_state = 0
    dp = {zero_state: 1}
    for row in range(n):
        is_last = (row == n - 1)
        new_dp = {}
        for top_int, cnt in dp.items():
            transitions = process_row(n, top_int, is_last)
            for bot_int, ways in transitions.items():
                new_dp[bot_int] = new_dp.get(bot_int, 0) + ways * cnt
        dp = new_dp
    return dp.get(zero_state, 0)

# Verify with known values
assert f(2) == 2, f"f(2) = {f(2)}, expected 2"
assert f(4) == 88, f"f(4) = {f(4)}, expected 88"

# Compute f(10)
t0 = time.time()
answer = f(10)
elapsed = time.time() - t0
print(f"f(10) = {answer}")
print(f"Computed in {elapsed:.2f}s")
assert answer == 112398351350823112
