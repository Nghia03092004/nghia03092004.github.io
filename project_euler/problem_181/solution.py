"""
Project Euler Problem 181: Investigating in How Many Ways Objects of Two
Different Colours Can Be Grouped

Count the number of bicolour partitions of (60, 40), i.e., the number of
unordered groupings of 60 black and 40 white objects into non-empty groups.
Uses two-dimensional unbounded knapsack DP.
"""


def solve(B=60, W=40):
    dp = [[0] * (W + 1) for _ in range(B + 1)]
    dp[0][0] = 1

    for db in range(B + 1):
        for dw in range(W + 1):
            if db == 0 and dw == 0:
                continue
            for b in range(db, B + 1):
                for w in range(dw, W + 1):
                    dp[b][w] += dp[b - db][w - dw]

    return dp[B][W]


if __name__ == "__main__":
    answer = solve()
assert answer == 83735848679360680
print(answer)
