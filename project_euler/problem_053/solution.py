"""
Problem 53: Combinatoric Selections

How many values of C(n, r) for 1 <= n <= 100 are greater than one million?
"""
from math import comb


def solve():
    count = 0
    threshold = 1_000_000
    for n in range(1, 101):
        for r in range(0, n + 1):
            if comb(n, r) > threshold:
                count += 1
    print(count)

solve()
