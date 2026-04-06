from fractions import Fraction
from math import comb

def solve():
    """
    Expected number of distinct colors when drawing 20 balls from 70
    (10 each of 7 colors).

    E = 7 * (1 - C(60,20)/C(70,20))
    """
    # Use exact rational arithmetic
    ratio = Fraction(comb(60, 20), comb(70, 20))
    expected = 7 * (1 - ratio)

    # Convert to decimal with 9 places
    # expected is a Fraction
    # We need to print with exactly 9 decimal places
    numerator = expected.numerator
    denominator = expected.denominator

    # Integer part
    integer_part = numerator // denominator
    remainder = numerator % denominator

    # Decimal part: multiply remainder by 10^9, divide by denominator
    decimal_part = (remainder * 10**9) // denominator

    # Check rounding
    next_digit = (remainder * 10**10) // denominator % 10
    if next_digit >= 5:
        decimal_part += 1

    print(f"{integer_part}.{decimal_part:09d}")

solve()
