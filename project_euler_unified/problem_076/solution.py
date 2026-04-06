"""
Problem 76: Counting Summations

How many different ways can 100 be written as a sum of at least two positive integers?
Answer: p(100) - 1 = 190569291
"""


def partition_count(n):
    """Compute p(n) via the standard unbounded-knapsack partition DP."""
    dp = [0] * (n + 1)
    dp[0] = 1
    for k in range(1, n + 1):
        for j in range(k, n + 1):
            dp[j] += dp[j - k]
    return dp[n]


def main():
    N = 100
    answer = partition_count(N) - 1  # exclude the trivial partition n = n
    print(answer)


if __name__ == "__main__":
    main()
