"""
Problem 92: Square Digit Chains

Count starting numbers below 10,000,000 that arrive at 89 by grouping
numbers according to their first square-digit-sum.
"""

def digit_square_sum(n):
    s = 0
    while n > 0:
        d = n % 10
        s += d * d
        n //= 10
    return s

def ends_at_89(n):
    while n not in (1, 89):
        n = digit_square_sum(n)
    return n == 89


def solve():
    digits = 7
    max_sum = digits * 81

    destinations = [False] * (max_sum + 1)
    for s in range(1, max_sum + 1):
        destinations[s] = ends_at_89(s)

    dp = [0] * (max_sum + 1)
    dp[0] = 1

    for _ in range(digits):
        next_dp = [0] * (max_sum + 1)
        for total, ways in enumerate(dp):
            if ways == 0:
                continue
            for digit in range(10):
                next_dp[total + digit * digit] += ways
        dp = next_dp

    return sum(dp[s] for s in range(max_sum + 1) if destinations[s])


if __name__ == "__main__":
    answer = solve()
    assert answer == 8581146
    print(answer)
