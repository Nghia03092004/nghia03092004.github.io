"""
Problem 101: Optimum Polynomial

Given u(n) = sum_{j=0}^{10} (-1)^j n^j, find the sum of all FITs
for the BOPs using the first k = 1, ..., 10 terms.
"""
from fractions import Fraction


def u(n):
    return sum((-n) ** j for j in range(11))


def lagrange_eval(points, x):
    k = len(points)
    result = Fraction(0)
    for i in range(k):
        xi, yi = points[i]
        term = Fraction(yi)
        for j in range(k):
            if j != i:
                xj = points[j][0]
                term *= Fraction(x - xj, xi - xj)
        result += term
    return result


def solve():
    total = 0
    for k in range(1, 11):
        points = [(i, u(i)) for i in range(1, k + 1)]
        fit = lagrange_eval(points, k + 1)
        total += int(fit)
    return total


if __name__ == "__main__":
    answer = solve()
    print(answer)
    assert answer == 37076114526
