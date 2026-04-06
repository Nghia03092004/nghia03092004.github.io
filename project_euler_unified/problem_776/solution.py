"""Project Euler 776: Digit Sum Division."""

from __future__ import annotations

from decimal import Decimal, localcontext
from fractions import Fraction
from typing import List, Tuple

TARGET = 1234567890123456789
TARGET_ANSWER = "9.627509725002e33"


def build_suffix_tables(length: int) -> Tuple[List[List[int]], List[List[int]], List[int]]:
    """Return count/value tables for suffixes with a fixed digit sum."""
    max_sum = 9 * length
    pow10 = [1] * (length + 1)
    for i in range(1, length + 1):
        pow10[i] = pow10[i - 1] * 10

    counts = [[0] * (max_sum + 1) for _ in range(length + 1)]
    totals = [[0] * (max_sum + 1) for _ in range(length + 1)]
    counts[0][0] = 1

    for digits in range(1, length + 1):
        place = pow10[digits - 1]
        for digit_sum in range(max_sum + 1):
            count = 0
            total = 0
            for digit in range(10):
                if digit > digit_sum:
                    break
                prev_count = counts[digits - 1][digit_sum - digit]
                if not prev_count:
                    continue
                count += prev_count
                total += digit * place * prev_count + totals[digits - 1][digit_sum - digit]
            counts[digits][digit_sum] = count
            totals[digits][digit_sum] = total

    return counts, totals, pow10


def digit_sum_weighted_totals(limit: int) -> List[int]:
    """Return T_s(limit) = sum_{n <= limit, d(n)=s} n for each digit sum s."""
    digits = [int(ch) for ch in str(limit)]
    length = len(digits)
    max_sum = 9 * length
    counts, totals, pow10 = build_suffix_tables(length)

    by_sum = [0] * (max_sum + 1)
    prefix_value = 0
    prefix_sum = 0

    for index, bound_digit in enumerate(digits):
        remaining = length - 1 - index
        place = pow10[remaining]

        for digit in range(bound_digit):
            next_prefix = prefix_value * 10 + digit
            for suffix_sum, suffix_count in enumerate(counts[remaining]):
                if not suffix_count:
                    continue
                total_sum = prefix_sum + digit + suffix_sum
                by_sum[total_sum] += next_prefix * place * suffix_count + totals[remaining][suffix_sum]

        prefix_value = prefix_value * 10 + bound_digit
        prefix_sum += bound_digit

    by_sum[prefix_sum] += limit
    return by_sum


def solve_fraction(limit: int = TARGET) -> Fraction:
    """Compute F(limit) exactly as a rational number."""
    answer = Fraction(0, 1)
    for digit_sum, total in enumerate(digit_sum_weighted_totals(limit)[1:], start=1):
        if total:
            answer += Fraction(total, digit_sum)
    return answer


def format_fraction(value: Fraction, digits_after_decimal: int = 12) -> str:
    """Format an exact rational in Project Euler scientific notation."""
    with localcontext() as ctx:
        ctx.prec = 80
        decimal_value = Decimal(value.numerator) / Decimal(value.denominator)
    mantissa, exponent = f"{decimal_value:.{digits_after_decimal}e}".split("e")
    return f"{mantissa}e{int(exponent)}"


def solve(limit: int = TARGET) -> str:
    return format_fraction(solve_fraction(limit))


def brute_force(limit: int) -> Fraction:
    total = Fraction(0, 1)
    for value in range(1, limit + 1):
        total += Fraction(value, sum(int(ch) for ch in str(value)))
    return total


def self_test() -> None:
    for limit in (1, 2, 10, 25, 100, 250):
        assert solve_fraction(limit) == brute_force(limit)

    assert solve_fraction(10) == 19
    assert format_fraction(solve_fraction(123)) == "1.187764610390e3"
    assert format_fraction(solve_fraction(12345)) == "4.855801996238e6"
    assert solve() == TARGET_ANSWER


if __name__ == "__main__":
    self_test()
    print(solve())
