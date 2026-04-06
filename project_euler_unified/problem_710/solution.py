"""
Problem 710: One Million Members
"""

def solve():
    # Count palindromic partitions
    # Using DP approach
    N = 1000
    # Partitions where each part appears even number of times
    dp = [0] * (N + 1)
    dp[0] = 1
    for k in range(1, N + 1):
        for j in range(N, 2*k - 1, -1):
            dp[j] += dp[j - 2*k]

    # Add center element options
    t = [0] * (N + 1)
    for n in range(N + 1):
        t[n] = dp[n]  # no center
        for c in range(1, n + 1):
            if n - c >= 0:
                t[n] += dp[n - c]

    # Find first n where t(n) > 10**6
    for n in range(1, N + 1):
        if t[n] > 10**6:
            print(n)
            return n
    return -1

answer = solve()
print(answer)
