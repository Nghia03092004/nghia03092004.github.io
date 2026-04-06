"""
Problem 129: Repunit Divisibility

Find the least n > 10^6 such that A(n) > 10^6,
where A(n) is the smallest k for which R(k) is divisible by n.
"""

def repunit_order(n):
    """Find smallest k such that R(k) divisible by n."""
    r = 1
    k = 1
    while r != 0:
        r = (10 * r + 1) % n
        k += 1
    return k

def solve():
    limit = 10**6

    n = limit + 1
    while True:
        if n % 2 != 0 and n % 5 != 0:
            if repunit_order(n) > limit:
                return n
        n += 1

def visualize():
    """Visualize A(n) for values near the answer."""
