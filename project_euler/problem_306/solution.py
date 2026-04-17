"""
Problem 306: Paper-strip Game

Two players alternate turns on a strip of n white squares. Each turn, a player
picks two contiguous white squares and paints them black. The first player
unable to move loses.

For 1 <= n <= 1,000,000, how many values of n allow the first player to force a win?

Solution: Use Sprague-Grundy theory. When two adjacent squares are painted at
position i on a strip of n, the strip splits into independent subgames of sizes
i and n-2-i. Grundy values become periodic with period 34 starting at n=53.

Answer: 852938
"""

def solve():
    LIMIT = 1_000_000
    COMPUTE = 200

    G = [0] * (COMPUTE + 1)
    for n in range(2, COMPUTE + 1):
        reachable = set()
        for i in range(n - 1):
            reachable.add(G[i] ^ G[n - 2 - i])
        mex = 0
        while mex in reachable:
            mex += 1
        G[n] = mex

    period = 34
    offset = 53

    # Verify
    for i in range(offset, COMPUTE + 1 - period):
        assert G[i] == G[i + period]

    # Count winning positions for n = 1 to offset-1
    count = sum(1 for i in range(1, offset) if G[i] != 0)

    # Wins per period
    wins_per_period = sum(1 for i in range(offset, offset + period) if G[i] != 0)

    remaining = LIMIT - offset + 1
    full = remaining // period
    leftover = remaining % period

    count += full * wins_per_period
    count += sum(1 for i in range(leftover) if G[offset + i] != 0)

    print(count)
