"""
Problem 207: Integer Partition Equations

4^t = 2^t + k. Let x = 2^t, then k = x^2 - x = x(x-1).
For integer x >= 2, each x gives a distinct positive integer k, so k takes values
2, 6, 12, 20, 30, 42, 56, ... (these are the "partitions").

Total partitions up to k: the number of integers x >= 2 with x(x-1) <= k.
For k = x(x-1), this is x - 1.

Perfect partitions: those where t is a positive integer, i.e., x = 2^m for m >= 1.
Perfect k values: 2, 12, 56, 240, 992, ...

P(k) = (# perfect partitions up to k) / (# total partitions up to k).

At k = x(x-1): P = floor(log2(x)) / (x - 1).
P is minimized at the left endpoint of each interval [x(x-1), (x+1)x - 1].

We need the least k with P(k) < 1/12345, i.e., the smallest x(x-1) where
(x-1) > 12345 * floor(log2(x)).
"""

import math

def solve():
    target = 12345
    for x in range(2, 10**8):
        m = x.bit_length() - 1  # floor(log2(x))
        if (x - 1) > target * m:
            return x * (x - 1)
    return None

answer = solve()
assert answer == 44043947822
print(answer)
