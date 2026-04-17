"""
Problem 175: Fractions Involving the Number of Different Ways
a Number Can Be Expressed as a Sum of Powers of 2

Find the shorthand notation for f(123456789/987654321).

The shorthand for fraction p/q (with p < q, both reduced) is:
1. Compute the continued fraction of q/p.
2. Ensure the last partial quotient is 1 (split if needed: [..., n] -> [..., n-1, 1]).
3. Reverse the sequence.

This gives the Calkin-Wilf tree path encoding.
"""

from math import gcd

def solve():
    p, q = 123456789, 987654321
    g = gcd(p, q)
    p //= g
    q //= g
    # p = 13717421, q = 109739369

    # Compute continued fraction of q/p
    a, b = q, p
    cf = []
    while b > 0:
        cf.append(a // b)
        a, b = b, a % b

    # Ensure last term is 1 by splitting [... n] -> [... n-1, 1]
    if cf[-1] > 1:
        cf[-1] -= 1
        cf.append(1)

    # Reverse to get the shorthand
    cf.reverse()

    print(",".join(str(x) for x in cf))

if __name__ == "__main__":
    solve()
