"""
Problem 972: Stern Diatomic Sequence

Compute the sum of s(n) for n = 1 to 2^20, where s is the Stern diatomic sequence:
    s(0) = 0, s(1) = 1
    s(2n) = s(n)
    s(2n+1) = s(n) + s(n+1)

Key results:
    - The sum over row k (indices 2^k to 2^(k+1)-1) equals 3^k
    - Total sum = sum of 3^k for k = 0..19 = (3^20 - 1) / 2
    - max(s(n)) for n <= 2^20 grows with the sequence depth
    - s(n)/s(n+1) enumerates all positive rationals exactly once

Methods:
    1. build_stern_sequence   — iterative DP construction
    2. row_sum_formula        — verify row k sum = 3^k
    3. max_per_row            — track maximum value in each binary row
    4. ratio_enumeration      — s(n)/s(n+1) as rational enumeration check
"""

from collections import Counter

def build_stern_sequence(N):
    """Build s[0..N] using the recurrence."""
    s = [0] * (N + 1)
    s[1] = 1
    for n in range(2, N + 1):
        if n % 2 == 0:
            s[n] = s[n // 2]
        else:
            s[n] = s[n // 2] + s[n // 2 + 1]
    return s

def verify_row_sums(s, num_rows=20):
    """Check that sum of s[2^k .. 2^(k+1)-1] == 3^k."""
    row_sums = []
    for k in range(num_rows):
        rs = sum(s[1 << k: 1 << (k + 1)])
        row_sums.append(rs)
        assert rs == 3 ** k, f"Row {k}: expected {3**k}, got {rs}"
    return row_sums

def max_per_row(s, num_rows=20):
    """Return the maximum s(n) in each binary row."""
    maxes = []
    for k in range(num_rows):
        lo = 1 << k
        hi = 1 << (k + 1)
        maxes.append(max(s[lo:hi]))
    return maxes

def consecutive_gcd_check(s, sample_size=1000):
    """Verify gcd(s(n), s(n+1)) == 1 for a sample of n values."""
    from math import gcd
    for n in range(1, min(sample_size, len(s) - 1)):
        assert gcd(s[n], s[n + 1]) == 1, f"gcd(s({n}), s({n+1})) != 1"
    return True

#  Verification
# Known first values: s = 0,1,1,2,1,3,2,3,1,4,3,5,2,5,3,4,...
_s = build_stern_sequence(15)
assert _s[:16] == [0, 1, 1, 2, 1, 3, 2, 3, 1, 4, 3, 5, 2, 5, 3, 4]

#  Main computation
N = 1 << 20  # 2^20 = 1048576
s = build_stern_sequence(N)
row_sums = verify_row_sums(s)
consecutive_gcd_check(s)

answer = sum(s[1:N + 1])
print(answer)

#  Visualization — 4-panel figure
