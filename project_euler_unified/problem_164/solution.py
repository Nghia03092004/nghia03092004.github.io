"""
Problem 164: Numbers for Which No Three Consecutive Digits Have a Sum Greater Than 9

Count 20-digit numbers where no three consecutive digits sum > 9.
Dynamic programming with state = (last two digits).
"""

def solve():
    DIGITS = 20

    # dp[a][b] = count of valid numbers ending in digits a, b
    dp = [[0]*10 for _ in range(10)]

    # Initialize: first two digits
    for a in range(1, 10):  # no leading zero
        for b in range(0, 10 - a):
            dp[a][b] = 1

    # Fill digits 3 through 20
    for pos in range(3, DIGITS + 1):
        ndp = [[0]*10 for _ in range(10)]
        for a in range(10):
            for b in range(10 - a):
                if dp[a][b] == 0:
                    continue
                for c in range(10 - a - b):
                    ndp[b][c] += dp[a][b]
        dp = ndp

    ans = sum(dp[a][b] for a in range(10) for b in range(10))
    print(ans)

solve()
