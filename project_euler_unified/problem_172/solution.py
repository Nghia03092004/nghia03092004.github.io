"""
Problem 172: Investigating Numbers with Few Repeated Digits

How many 18-digit numbers n (no leading zero) have each digit appearing
at most three times?

Method: Sequential digit DP using binomial coefficients.
"""

from math import comb

def count_strings(L, max_freq):
    """Count L-length strings over {0,...,9} with digit i at most max_freq[i] times."""
    dp = [0] * (L + 1)
    dp[0] = 1
    for dig in range(10):
        ndp = [0] * (L + 1)
        for j in range(L + 1):
            if dp[j] == 0:
                continue
            for c in range(max_freq[dig] + 1):
                if j + c > L:
                    break
                ndp[j + c] += dp[j] * comb(L - j, c)
        dp = ndp
    return dp[L]

def solve():
    T = count_strings(18, [3] * 10)
    mf = [3] * 10
    mf[0] = 2
    T0 = count_strings(17, mf)
    print(T - T0)

if __name__ == "__main__":
    solve()
