"""
Problem 259: Reachable Numbers

From the string "123456789", insert +, -, *, / between groups of consecutive
digits (with arbitrary parenthesization) to produce values. Find the sum of
all positive integers reachable.

Method: Interval DP with exact rational arithmetic (fractions).
R(i,j) = set of all rationals from digits i..j.

Answer: 20101196798
"""

from fractions import Fraction
from functools import lru_cache

def solve():
    digits = list(range(1, 10))  # 1..9, 0-indexed as 0..8

    # dp[i][j] = set of Fraction values from digits[i..j]
    dp = [[None] * 9 for _ in range(9)]

    def get(i, j):
        if dp[i][j] is not None:
            return dp[i][j]

        s = set()

        # Concatenated number
        num = 0
        for k in range(i, j + 1):
            num = num * 10 + digits[k]
        s.add(Fraction(num))

        # Split at every point
        for k in range(i, j):
            left = get(i, k)
            right = get(k + 1, j)
            for a in left:
                for b in right:
                    s.add(a + b)
                    s.add(a - b)
                    s.add(a * b)
                    if b != 0:
                        s.add(a / b)

        dp[i][j] = s
        return s

    reachable = get(0, 8)

    ans = sum(f.numerator for f in reachable
              if f.denominator == 1 and f.numerator > 0)
    print(ans)

if __name__ == "__main__":
    solve()
