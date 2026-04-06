"""
Project Euler Problem 162: Hexadecimal Numbers

Count hexadecimal numbers with at most 16 digits containing all of 0, 1, and A
at least once.  Uses the inclusion-exclusion principle.
"""


def solve():
    total = 0
    for n in range(1, 17):
        S = 15 * 16 ** (n - 1)
        A0 = 15 ** n
        A1 = 14 * 15 ** (n - 1)
        AA = 14 * 15 ** (n - 1)
        A01 = 14 ** n
        A0A = 14 ** n
        A1A = 13 * 14 ** (n - 1)
        A01A = 13 ** n
        bad = A0 + A1 + AA - A01 - A0A - A1A + A01A
        total += S - bad
    print(f"{total:X}")


solve()
