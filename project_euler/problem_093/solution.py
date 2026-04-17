"""
Project Euler Problem 93: Arithmetic Expressions

Find the set of four digits {a,b,c,d} (from {1,...,9}) for which the longest
consecutive run of positive integers 1..n can be obtained using +,-,*,/ and
parentheses, with each digit used exactly once.
"""

from fractions import Fraction
from functools import lru_cache
from itertools import combinations


def next_values(a, b):
    values = {a + b, a - b, b - a, a * b}
    if b != 0:
        values.add(a / b)
    if a != 0:
        values.add(b / a)
    return values


@lru_cache(maxsize=None)
def reachable_values(state):
    if len(state) == 1:
        return frozenset(state)

    values = list(state)
    results = set()

    for i in range(len(values)):
        for j in range(i + 1, len(values)):
            a = values[i]
            b = values[j]
            rest = [values[k] for k in range(len(values)) if k not in (i, j)]

            for value in next_values(a, b):
                next_state = tuple(sorted(rest + [value]))
                results.update(reachable_values(next_state))

    return frozenset(results)


def solve():
    best_n = 0
    best_digits = None

    for digits in combinations(range(1, 10), 4):
        state = tuple(Fraction(digit) for digit in digits)
        reachable = reachable_values(state)
        integers = {
            value.numerator
            for value in reachable
            if value.denominator == 1 and value.numerator > 0
        }

        n = 0
        while (n + 1) in integers:
            n += 1

        if n > best_n:
            best_n = n
            best_digits = digits

    return ''.join(map(str, best_digits))


if __name__ == "__main__":
    answer = solve()
    assert answer == "1258"
    print(answer)
