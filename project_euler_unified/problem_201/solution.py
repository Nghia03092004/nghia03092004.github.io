"""
Problem 201: Subsets with a Unique Sum

For S = {1^2, 2^2, ..., 100^2}, find the sum of all integers that are the
sum of exactly one subset of S of size 50.

We use a 3-state DP: 0 = unreachable, 1 = exactly one subset, 2 = two or more.
"""

def solve():
    N = 100
    K = 50
    elements = [(i+1)**2 for i in range(N)]

    max_sum = sum(elements[K:])  # sum of 51^2..100^2 = 295425

    # dp[j][s] in {0, 1, 2}
    # Use a list of bytearray for efficiency
    dp = [bytearray(max_sum + 1) for _ in range(K + 1)]
    dp[0][0] = 1

    for i, ai in enumerate(elements):
        # Iterate j in reverse
        for j in range(min(i + 1, K), 0, -1):
            for s in range(max_sum, ai - 1, -1):
                if dp[j-1][s - ai]:
                    val = dp[j][s] + dp[j-1][s - ai]
                    dp[j][s] = min(val, 2)

    ans = sum(s for s in range(max_sum + 1) if dp[K][s] == 1)
    print(ans)

if __name__ == "__main__":
    solve()
