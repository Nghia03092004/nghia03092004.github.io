def solve():
    W = 32
    H = 10

    # Generate all row tilings as sets of crack positions
    rows = []

    def generate(pos, cracks):
        if pos == W:
            rows.append(frozenset(cracks))
            return
        # Try 2-brick
        if pos + 2 <= W:
            new_cracks = cracks + ([pos + 2] if pos + 2 < W else [])
            generate(pos + 2, new_cracks)
        # Try 3-brick
        if pos + 3 <= W:
            new_cracks = cracks + ([pos + 3] if pos + 3 < W else [])
            generate(pos + 3, new_cracks)

    generate(0, [])
    m = len(rows)

    # Build compatibility: two rows are compatible if they share no crack position
    compat = [[] for _ in range(m)]
    for i in range(m):
        for j in range(m):
            if rows[i].isdisjoint(rows[j]):
                compat[i].append(j)

    # DP: dp[i] = number of walls of current height ending with row i
    dp = [1] * m  # height 1

    for h in range(2, H + 1):
        ndp = [0] * m
        for i in range(m):
            for j in compat[i]:
                ndp[i] += dp[j]
        dp = ndp

    answer = sum(dp)
    print(answer)

if __name__ == "__main__":
    solve()
