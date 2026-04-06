"""
Project Euler 637: Flexible Digit Sum

f(n,B) = number of digit-sum steps to reach a single digit in base B.
g(n,B1,B2) = sum of i <= n where f(i,B1) == f(i,B2).
Find g(10^7, 10, 3).
"""

def digit_sum(n, base):
    """Compute the sum of digits of n in the given base."""
    s = 0
    while n > 0:
        s += n % base
        n //= base
    return s

def f(n, base):
    """Number of digit-sum steps to reach a single digit in given base."""
    steps = 0
    while n >= base:
        n = digit_sum(n, base)
        steps += 1
    return steps

def solve():
    N = 10**7
    result = 0
    for i in range(1, N + 1):
        if f(i, 10) == f(i, 3):
            result += i
    print(result)

if __name__ == "__main__":
    solve()
