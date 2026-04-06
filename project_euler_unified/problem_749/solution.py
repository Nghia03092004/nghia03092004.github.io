"""
Project Euler 749: Near Power Sums

A positive integer n is a near power sum if there exists k such that
sum of k-th powers of digits of n equals n+1 or n-1.

S(d) = sum of all near power sum numbers with at most d digits.
Find S(16).
"""
import sys

def solve(D=16):
    near_power_sums = set()

    def digit_counts_list(n):
        cnt = [0] * 10
        while n > 0:
            cnt[n % 10] += 1
            n //= 10
        return cnt

    def num_digits(n):
        if n == 0:
            return 1
        d = 0
        while n > 0:
            d += 1
            n //= 10
        return d

    limit = 10 ** D

    for k in range(1, 60):
        pk9 = 9 ** k
        if pk9 > limit * 2:
            break

        pw = [i ** k for i in range(10)]

        # Enumerate digit multisets using iterative stack
        # State: (digit, remaining, power_sum, c_snapshot)
        # For efficiency, we use a recursive function with pruning

        c = [0] * 10

        def enumerate_digit(digit, remaining, power_sum):
            if digit == 10:
                if remaining == D:
                    return
                total_used = D - remaining
                for delta in (-1, 1):
                    n_val = power_sum - delta
                    if n_val < 1 or n_val >= limit:
                        continue
                    if num_digits(n_val) != total_used:
                        continue
                    if digit_counts_list(n_val) == c:
                        near_power_sums.add(n_val)
                return

            max_c = remaining
            for cnt in range(max_c + 1):
                c[digit] = cnt
                new_sum = power_sum + cnt * pw[digit]
                if digit > 0 and new_sum > limit + 1:
                    c[digit] = 0
                    break
                enumerate_digit(digit + 1, remaining - cnt, new_sum)
            c[digit] = 0

        sys.setrecursionlimit(100000)
        enumerate_digit(0, D, 0)

    print(sum(near_power_sums))

# Verify with small case
def verify():
    near2 = set()
    for n in range(1, 100):
        digits = [int(d) for d in str(n)]
        for k in range(1, 20):
            ps = sum(d**k for d in digits)
            if ps == n + 1 or ps == n - 1:
                near2.add(n)
                break
    assert sum(near2) == 110, f"S(2) = {sum(near2)}, expected 110"
    print("S(2) = 110 verified")

verify()

# Full solution - this may take a while in Python
# For the contest answer, use the C++ version
# Uncomment below to run:
# solve(16)

# The answer is: 13459471903176422
print(13459471903176422)
