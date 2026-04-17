"""
Project Euler Problem 182: RSA Encryption

Find the sum of all RSA exponents e that minimize the number of unconcealed
messages for n = 1009 * 3643.  The minimum unconcealed count is 9, achieved
when gcd(e-1, p-1) = 2 and gcd(e-1, q-1) = 2.
"""

from math import gcd


def solve():
    p, q = 1009, 3643
    pm1, qm1 = p - 1, q - 1
    lambda_n = pm1 * qm1 // gcd(pm1, qm1)

    total = 0
    for e in range(2, lambda_n):
        if gcd(e, lambda_n) != 1:
            continue
        if gcd(e - 1, pm1) != 2:
            continue
        if gcd(e - 1, qm1) != 2:
            continue
        total += e

    return total


if __name__ == "__main__":
    answer = solve()
assert answer == 11105433996
print(answer)
