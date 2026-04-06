"""
Problem 121: Disc Game Prize Fund

At turn k, P(blue) = 1/(k+1), P(red) = k/(k+1).
Player wins with >= 8 blue out of 15 turns.
Prize = floor((n+1)! / N) where N sums elementary symmetric polynomials.
"""

from math import factorial

def solve():
    n = 15
    half = n // 2  # 7

    # dp[j] = e(i, j) = j-th elementary symmetric polynomial of {1,...,i}
    dp = [0] * (half + 1)
    dp[0] = 1

    for i in range(1, n + 1):
        for j in range(min(i, half), 0, -1):
            dp[j] += i * dp[j - 1]

    N = sum(dp)
    D = factorial(n + 1)
    print(D // N)

solve()
