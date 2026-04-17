"""
Problem 52: Permuted Multiples

Find the smallest positive integer x such that 2x, 3x, 4x, 5x, and 6x
contain the same digits as x.
"""


def sorted_digits(n):
    return sorted(str(n))


def solve():
    x = 1
    while True:
        sd = sorted_digits(x)
        if all(sorted_digits(k * x) == sd for k in range(2, 7)):
            print(x)
            return
        x += 1

solve()
