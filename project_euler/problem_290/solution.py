"""
Problem 290: Digital Signature

Count integers 0 <= n < 10^18 where S(n) = S(137*n).

Approach: Digit DP processing digits from least to most significant.
State: (carry, diff) where carry is from the 137*n multiplication and
diff = S(n) - S(137n) accumulated so far.
"""

def solve():
    DIGITS = 18
    MAX_CARRY = 137  # 0..136
    DIFF_OFFSET = 162
    DIFF_RANGE = 325

    # dp[carry][diff+offset] = count
    dp = [[0] * DIFF_RANGE for _ in range(MAX_CARRY)]
    dp[0][DIFF_OFFSET] = 1

    for pos in range(DIGITS):
        ndp = [[0] * DIFF_RANGE for _ in range(MAX_CARRY)]
        for carry in range(MAX_CARRY):
            for di in range(DIFF_RANGE):
                cnt = dp[carry][di]
                if cnt == 0:
                    continue
                for y in range(10):
                    val = 137 * y + carry
                    new_carry = val // 10
                    v = val % 10
                    new_di = di + (y - v)
                    if 0 <= new_di < DIFF_RANGE and new_carry < MAX_CARRY:
                        ndp[new_carry][new_di] += cnt
        dp = ndp

    # After processing all digits of n, remaining carry c gives additional
    # upper digits of 137*n. We need diff = digit_sum(carry).
    def digit_sum(x):
        s = 0
        while x > 0:
            s += x % 10
            x //= 10
        return s

    result = 0
    for carry in range(MAX_CARRY):
        ds = digit_sum(carry)
        di = DIFF_OFFSET + ds
        if 0 <= di < DIFF_RANGE:
            result += dp[carry][di]
    print(result)

if __name__ == "__main__":
    solve()

    # Optional visualization: distribution of digit sum differences
