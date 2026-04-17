"""
Problem 178: Step Numbers

Count pandigital step numbers (containing all digits 0-9) with up to 40 digits.
DP approach with state (last_digit, digit_bitmask).
"""


def solve():

    MAXN = 40
    FULL = (1 << 10) - 1  # 1023 = all digits 0-9 present

    # dp[d][mask] = count of step numbers ending in digit d with digit set mask
    dp = [[0] * 1024 for _ in range(10)]

    # Base: 1-digit numbers (no leading zero)
    for d in range(1, 10):
        dp[d][1 << d] = 1

    answer = 0

    # Check 1-digit pandigital (impossible)
    for d in range(10):
        answer += dp[d][FULL]

    for n in range(1, MAXN):
        ndp = [[0] * 1024 for _ in range(10)]
        for d in range(10):
            for mask in range(1024):
                if dp[d][mask] == 0:
                    continue
                val = dp[d][mask]
                if d >= 1:
                    ndp[d-1][mask | (1 << (d-1))] += val
                if d <= 8:
                    ndp[d+1][mask | (1 << (d+1))] += val
        dp = ndp

        for d in range(10):
            answer += dp[d][FULL]

    return answer


answer = solve()
assert answer == 126461847755
print(answer)
